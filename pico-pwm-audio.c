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
#define SIN 14
#define SQUARE 13
#define TRIANGLE 12
#define SAWTOOTH 11
#define R_SAWTOOTH 10

#include "waves.h"

int wav_position = 0;
float adc_value=0;
const float conversionfactor=1.65f/(1<<12);
int timeInterval=75;
bool vibrato = false; //vibrato on or off
//float frequencies[]={256};
float frequency=WAV_FREQUENCY;
float currentF = WAV_FREQUENCY;
int freqNum=0;
int interval=0;
//int FreqCount = round(sizeof(frequencies)/sizeof(frequencies[0]))-1;
float clkDiv=2.0f;
const float vibsize = WAV_FREQUENCY/12;
float vibchangeParam = vibsize/12;
int button = 0;
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
void rawHandler(){
    if(gpio_get_irq_event_mask(VIBRATO_PIN) & GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL){
        gpio_acknowledge_irq(VIBRATO_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL);
        if(vibrato){
            vibrato=false;
            updateClockDiv(clockDivChange(frequency));
        }else{
            vibrato=true;
            updateClockDiv(clockDivChange(frequency));
        }
    }
    if(gpio_get_irq_event_mask(SIN) & GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL){
        gpio_acknowledge_irq(SIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL);
        button=0;
    }
    if(gpio_get_irq_event_mask(SQUARE) & GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL){
        gpio_acknowledge_irq(SQUARE, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL);
        button=1;
    }
    if(gpio_get_irq_event_mask(TRIANGLE) & GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL){
        gpio_acknowledge_irq(TRIANGLE, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL);
        button=2;
    }
    if(gpio_get_irq_event_mask(SAWTOOTH) & GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL){
        gpio_acknowledge_irq(SAWTOOTH, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL);
        button=3;
    }
    if(gpio_get_irq_event_mask(R_SAWTOOTH) & GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL){
        gpio_acknowledge_irq(R_SAWTOOTH, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL);
        button=4;
    }
}
void pwm_interrupt_handler() {
    pwm_clear_irq(pwm_gpio_to_slice_num(AUDIO_PIN));
    if(vibrato){
        if (wav_position < (WAV_DATA_LENGTH<<3) - 1) { 
            // set pwm level 
            // allow the pwm value to repeat for 8 cycles this is >>3 
            if(button==0){
                pwm_set_gpio_level(AUDIO_PIN, SIN_WAV_DATA[wav_position>>3]);
            }
            else if(button==1){
                pwm_set_gpio_level(AUDIO_PIN, SQR_WAV_DATA[wav_position>>3]);
            }
            else if(button==2){
                pwm_set_gpio_level(AUDIO_PIN, TRI_WAV_DATA[wav_position>>3]);
            }
            else if(button==3){
                pwm_set_gpio_level(AUDIO_PIN, SAW_WAV_DATA[wav_position>>3]);
            }
            else if(button==4){
                pwm_set_gpio_level(AUDIO_PIN, R_SAW_WAV_DATA[wav_position>>3]);
            }
              
            wav_position++;
        } else {
            // reset to start
            wav_position = 0;
            if(vibUP){
                if(currentF<frequency+vibsize){
                    currentF+=vibchangeParam;
                } else{
                    vibUP=false;
                }
                updateClockDiv(clockDivChange(currentF));
            } else{
                if(currentF>frequency-vibsize){
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
            if(button==0){
                pwm_set_gpio_level(AUDIO_PIN, SIN_WAV_DATA[wav_position>>3]);
            }
            else if(button==1){
                pwm_set_gpio_level(AUDIO_PIN, SQR_WAV_DATA[wav_position>>3]);
            }
            else if(button==2){
                pwm_set_gpio_level(AUDIO_PIN, TRI_WAV_DATA[wav_position>>3]);
            }
            else if(button==3){
                pwm_set_gpio_level(AUDIO_PIN, SAW_WAV_DATA[wav_position>>3]);
            }
            else if(button==4){
                pwm_set_gpio_level(AUDIO_PIN, R_SAW_WAV_DATA[wav_position>>3]);
            }

            wav_position++;
        } else {
            adc_value=(adc_read())*conversionfactor;
            frequency=WAV_FREQUENCY*adc_value;
            // reset to start
            wav_position = 0;
            updateClockDiv(clockDivChange(frequency));
            
        }
    }
    
    
}

int main(void) {
    /* Overclocking for fun but then also so the system clock is a 
     * multiple of typical audio sampling rates.
     */
    stdio_init_all();
    adc_init();
    adc_gpio_init(ADC_PIN);
    adc_select_input(0);

    gpio_init(VIBRATO_PIN);
    gpio_set_dir(VIBRATO_PIN,GPIO_IN);
    gpio_set_irq_enabled_with_callback(VIBRATO_PIN,GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL,true,rawHandler);
    
    gpio_init(SIN);
    gpio_set_dir(SIN,GPIO_IN);
    gpio_set_irq_enabled_with_callback(SIN,GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL,true,rawHandler);
    
    gpio_init(SQUARE);
    gpio_set_dir(SQUARE,GPIO_IN);
    gpio_set_irq_enabled_with_callback(SQUARE,GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL,true,rawHandler);

    gpio_init(TRIANGLE);
    gpio_set_dir(TRIANGLE,GPIO_IN);
    gpio_set_irq_enabled_with_callback(TRIANGLE,GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL,true,rawHandler);

    gpio_init(SAWTOOTH);
    gpio_set_dir(SAWTOOTH,GPIO_IN);
    gpio_set_irq_enabled_with_callback(SAWTOOTH,GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL,true,rawHandler);

    gpio_init(R_SAWTOOTH);
    gpio_set_dir(R_SAWTOOTH,GPIO_IN);
    gpio_set_irq_enabled_with_callback(R_SAWTOOTH,GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL,true,rawHandler);
    
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
