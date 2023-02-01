#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"   // stdlib 
#include "hardware/irq.h"  // interrupts
#include "hardware/pwm.h"  // pwm 
#include "hardware/sync.h" // wait for interrupt 
#include "hardware/adc.h"
 
// Audio PIN is to match some of the design guide shields. 
#define AUDIO_PIN 28  // you can change this to whatever you like
#define ADC_PIN 26
#define VIBRATO_PIN 15
#include "Csine.h"

int wav_position = 0;
float adc_value=0;
const float conversionfactor=1.65f/(1<<12);
int timeInterval=75;
bool vibrato = false; //vibrato on or off
float frequencies[]={256};
float currentF = WAV_FREQUENCY;
int freqNum=0;
int interval=0;
int FreqCount = round(sizeof(frequencies)/sizeof(frequencies[0]))-1;
float clkDiv=2.0f;
float vibsize = WAV_FREQUENCY/12;
float vibchangeParam = (vibsize)/12;

bool vibUP=true;
double sine_wave_y(double x) {
    return sin(x);
}

float clockDivChange( float newFrequency){
    return (WAV_FREQUENCY/newFrequency)*2.0f;
}
void updateClockDiv(float clkDiv){
    int pin_slice = pwm_gpio_to_slice_num(AUDIO_PIN);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, clkDiv); 
    pwm_config_set_wrap(&config, 250); 
    pwm_init(pin_slice, &config, true);
    pwm_set_gpio_level(AUDIO_PIN, 0);
}
void vibratoHandler(){
    if(gpio_get_irq_event_mask(VIBRATO_PIN) & GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL){
        gpio_acknowledge_irq(VIBRATO_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL);
        if(vibrato){
            vibrato=false;
            updateClockDiv(clockDivChange(frequencies[0]));
        }else{
            vibrato=true;
            updateClockDiv(clockDivChange(frequencies[0]));
        }
    }
}


void pwm_interrupt_handler() {
    pwm_clear_irq(pwm_gpio_to_slice_num(AUDIO_PIN));
    adc_value=(adc_read())*conversionfactor;
    if(vibrato){
        if (wav_position < (WAV_DATA_LENGTH<<3) - 1) { 
            // set pwm level 
            // allow the pwm value to repeat for 8 cycles this is >>3 
            pwm_set_gpio_level(AUDIO_PIN, WAV_DATA[wav_position>>3]);  
            wav_position++;
        } else {
            // reset to start
            wav_position = 0;
            if(vibUP){
                if(currentF<frequencies[0]+vibsize){
                    currentF+=vibchangeParam;
                } else{
                    vibUP=false;
                }
                updateClockDiv(clockDivChange(currentF));
            } else{
                if(currentF>frequencies[0]-vibsize){
                    currentF-=vibchangeParam;
                } else{
                    vibUP=true;
                }
                updateClockDiv(clockDivChange(currentF));
            }
            
        }
    }else{
        if (wav_position < (WAV_DATA_LENGTH<<3) - 1) { 
            // set pwm level 
            // allow the pwm value to repeat for 8 cycles this is >>3 
            pwm_set_gpio_level(AUDIO_PIN, WAV_DATA[wav_position>>3]);  
            wav_position++;
        } else {
            // reset to start
            wav_position = 0;
            updateClockDiv(clockDivChange(frequencies[0]*adc_value));
            
        }
    }
    
    
}

int main(void) {
    /* Overclocking for fun but then also so the system clock is a 
     * multiple of typical audio sampling rates.
     */
    stdio_init_all();
    adc_init()
    adc_gpio_init(ADC_PIN);
    adc_select_input(0);

    gpio_init(VIBRATO_PIN);
    gpio_set_dir(VIBRATO_PIN,GPIO_IN);
    gpio_set_irq_enabled(VIBRATO_PIN,GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL,true);
    gpio_add_raw_irq_handler(VIBRATO_PIN, vibratoHandler);
    irq_set_enabled(IO_IRQ_BANK0, true);
    
    set_sys_clock_khz(176000, true); 
    gpio_set_function(AUDIO_PIN, GPIO_FUNC_PWM);
    int audio_pin_slice = pwm_gpio_to_slice_num(AUDIO_PIN);
    // Setup PWM interrupt to fire when PWM cycle is complete
    pwm_clear_irq(audio_pin_slice);
    pwm_set_irq_enabled(audio_pin_slice, true);
    // set the handle function above
    irq_set_exclusive_handler(PWM_IRQ_WRAP, pwm_interrupt_handler); 
    irq_set_enabled(PWM_IRQ_WRAP, true);

    // Setup PWM for audio output
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, clkDiv); 
    pwm_config_set_wrap(&config, 250); 
    pwm_init(audio_pin_slice, &config, true);

    pwm_set_gpio_level(AUDIO_PIN, 0);

    while(1) {
        __wfi(); // Wait for Interrupt
    }
}
