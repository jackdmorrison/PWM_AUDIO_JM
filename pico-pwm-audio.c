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
#define WAVEBUTTON 10

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
int value=0;
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
// void callback(){
//     if(gpio_get_irq_event_mask(VIBRATO_PIN) & GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL){
//         gpio_acknowledge_irq(VIBRATO_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL);
//         if(vibrato){
//             vibrato=false;
//             updateClockDiv(clockDivChange(frequency));
//         }else{
//             vibrato=true;
//             updateClockDiv(clockDivChange(frequency));
//         }
//     }
// }
void rawHandler(){
    if(gpio_get_irq_event_mask(WAVEBUTTON) & GPIO_IRQ_EDGE_RISE ){
        gpio_acknowledge_irq(WAVEBUTTON, GPIO_IRQ_EDGE_RISE );

        if(button<16){
            button++;
        }
        else{
            button=0;
        }
    }
}
void pwm_interrupt_handler() {
    pwm_clear_irq(pwm_gpio_to_slice_num(AUDIO_PIN));
        if (wav_position < (WAV_DATA_LENGTH<<3) - 1) { 
            // set pwm level 
            // allow the pwm value to repeat for 8 cycles this is >>3 
            switch (button){
                case 0: //base Harmonic
                    value=HARMONIC1_WAV_DATA[wav_position>>3];
                    break;
                case 1: //+harmonic1+2
                    value=round((HARMONIC1_WAV_DATA[wav_position>>3]+HARMONIC2_WAV_DATA[wav_position>>3])/2);
                    break;
                case 2: //+harmonic1+2+3
                    value=round((HARMONIC1_WAV_DATA[wav_position>>3]+HARMONIC2_WAV_DATA[wav_position>>3]+HARMONIC3_WAV_DATA[wav_position>>3])/3);
                    break;
                case 3: //+harmonic1+2+3+4
                    value=round((HARMONIC1_WAV_DATA[wav_position>>3]+HARMONIC2_WAV_DATA[wav_position>>3]+HARMONIC3_WAV_DATA[wav_position>>3]+HARMONIC4_WAV_DATA[wav_position>>3])/4);
                    break;
                case 4: //+harmonic1+2+3+4+5
                    value=round((HARMONIC1_WAV_DATA[wav_position>>3]+HARMONIC2_WAV_DATA[wav_position>>3]+HARMONIC3_WAV_DATA[wav_position>>3]+HARMONIC4_WAV_DATA[wav_position>>3]+HARMONIC5_WAV_DATA[wav_position>>3])/5);
                    break;
                case 5: //+harmonic1+2+3+4+5+6
                    value=round((HARMONIC1_WAV_DATA[wav_position>>3]+HARMONIC2_WAV_DATA[wav_position>>3]+HARMONIC3_WAV_DATA[wav_position>>3]+HARMONIC4_WAV_DATA[wav_position>>3]+HARMONIC5_WAV_DATA[wav_position>>3]+HARMONIC6_WAV_DATA[wav_position>>3])/6);
                    break;
                case 6: //+harmonic1+2+3+4+5+6+7
                    value=round((HARMONIC1_WAV_DATA[wav_position>>3]+HARMONIC2_WAV_DATA[wav_position>>3]+HARMONIC3_WAV_DATA[wav_position>>3]+HARMONIC4_WAV_DATA[wav_position>>3]+HARMONIC5_WAV_DATA[wav_position>>3]+HARMONIC6_WAV_DATA[wav_position>>3]+HARMONIC7_WAV_DATA[wav_position>>3])/7);
                    break;
                case 7: //+harmonic1+2+3+4+5+6+7+8
                    value=round((HARMONIC1_WAV_DATA[wav_position>>3]+HARMONIC2_WAV_DATA[wav_position>>3]+HARMONIC3_WAV_DATA[wav_position>>3]+HARMONIC4_WAV_DATA[wav_position>>3]+HARMONIC5_WAV_DATA[wav_position>>3]+HARMONIC6_WAV_DATA[wav_position>>3]+HARMONIC7_WAV_DATA[wav_position>>3]+HARMONIC8_WAV_DATA[wav_position>>3])/8);
                    break;
                case 8: //+harmonic1+2+3+4+5+6+7+8+9
                    value=round((HARMONIC1_WAV_DATA[wav_position>>3]+HARMONIC2_WAV_DATA[wav_position>>3]+HARMONIC3_WAV_DATA[wav_position>>3]+HARMONIC4_WAV_DATA[wav_position>>3]+HARMONIC5_WAV_DATA[wav_position>>3]+HARMONIC6_WAV_DATA[wav_position>>3]+HARMONIC7_WAV_DATA[wav_position>>3]+HARMONIC8_WAV_DATA[wav_position>>3]+HARMONIC9_WAV_DATA[wav_position>>3])/9);
                    break;
                case 9: //+harmonic1+2+3+4+5+6+7+9
                    value=round((HARMONIC1_WAV_DATA[wav_position>>3]+HARMONIC2_WAV_DATA[wav_position>>3]+HARMONIC3_WAV_DATA[wav_position>>3]+HARMONIC4_WAV_DATA[wav_position>>3]+HARMONIC5_WAV_DATA[wav_position>>3]+HARMONIC6_WAV_DATA[wav_position>>3]+HARMONIC7_WAV_DATA[wav_position>>3]+HARMONIC9_WAV_DATA[wav_position>>3])/8);
                    break;
                case 10: //+harmonic1+2+3+4+5+7+9
                    value=round((HARMONIC1_WAV_DATA[wav_position>>3]+HARMONIC2_WAV_DATA[wav_position>>3]+HARMONIC3_WAV_DATA[wav_position>>3]+HARMONIC4_WAV_DATA[wav_position>>3]+HARMONIC5_WAV_DATA[wav_position>>3]+HARMONIC7_WAV_DATA[wav_position>>3]+HARMONIC9_WAV_DATA[wav_position>>3])/7);
                    break;
                case 11: //+harmonic1+2+3+5+7+9
                    value=round((HARMONIC1_WAV_DATA[wav_position>>3]+HARMONIC2_WAV_DATA[wav_position>>3]+HARMONIC3_WAV_DATA[wav_position>>3]+HARMONIC5_WAV_DATA[wav_position>>3]+HARMONIC7_WAV_DATA[wav_position>>3]+HARMONIC9_WAV_DATA[wav_position>>3])/6);
                    break; 
                case 12: //+harmonic1+3+5+7+9
                    value=round((HARMONIC1_WAV_DATA[wav_position>>3]+HARMONIC3_WAV_DATA[wav_position>>3]+HARMONIC5_WAV_DATA[wav_position>>3]+HARMONIC7_WAV_DATA[wav_position>>3]+HARMONIC9_WAV_DATA[wav_position>>3])/5);
                    break;
                case 13: //+harmonic1+2+5+7+9
                    value=round((HARMONIC1_WAV_DATA[wav_position>>3]+HARMONIC2_WAV_DATA[wav_position>>3]+HARMONIC5_WAV_DATA[wav_position>>3]+HARMONIC7_WAV_DATA[wav_position>>3]+HARMONIC9_WAV_DATA[wav_position>>3])/5);
                    break;
                case 14: //+harmonic1+2+4+7+9
                    value=round((HARMONIC1_WAV_DATA[wav_position>>3]+HARMONIC2_WAV_DATA[wav_position>>3]+HARMONIC4_WAV_DATA[wav_position>>3]+HARMONIC7_WAV_DATA[wav_position>>3]+HARMONIC9_WAV_DATA[wav_position>>3])/5);
                    break;
                case 15: //+harmonic1+2+4+6+9
                    value=round((HARMONIC1_WAV_DATA[wav_position>>3]+HARMONIC2_WAV_DATA[wav_position>>3]+HARMONIC4_WAV_DATA[wav_position>>3]+HARMONIC6_WAV_DATA[wav_position>>3]+HARMONIC9_WAV_DATA[wav_position>>3])/5);
                    break;
                case 16: //+harmonic1+2+4+6+8
                    value=round((HARMONIC1_WAV_DATA[wav_position>>3]+HARMONIC2_WAV_DATA[wav_position>>3]+HARMONIC4_WAV_DATA[wav_position>>3]+HARMONIC6_WAV_DATA[wav_position>>3]+HARMONIC8_WAV_DATA[wav_position>>3])/5);
                    break;
            }
            pwm_set_gpio_level(AUDIO_PIN, value);
            wav_position++;
        } else {
            adc_value=(adc_read())*conversionfactor;
            frequency=WAV_FREQUENCY*adc_value;
            // reset to start
            wav_position = 0;
            updateClockDiv(clockDivChange(frequency));
            
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

    // gpio_init(VIBRATO_PIN);
    // gpio_set_dir(VIBRATO_PIN,GPIO_IN);
    // gpio_set_irq_enabled(VIBRATO_PIN,GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL,true);
    // gpio_add_raw_irq_handler(VIBRATO_PIN, callback);
    gpio_init(WAVEBUTTON);
    gpio_set_dir(WAVEBUTTON,GPIO_IN);
    gpio_set_irq_enabled(WAVEBUTTON,GPIO_IRQ_EDGE_RISE ,true);
    gpio_add_raw_irq_handler(WAVEBUTTON, rawHandler );
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
