#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"    
#include "hardware/irq.h"  
#include "hardware/pwm.h"  
#include "hardware/sync.h" 
#include "hardware/adc.h"
  
#define AUDIO_PIN 28  
#define ADC_PIN 26
#define VIBRATO_PIN 15
#define WAVEBUTTON 10
#define GATE 0

#include "waves.h"

int wav_position = 0;
float adc_value=0;
const float conversionfactor=1.0f/(1<<12);
int timeInterval=75;
bool vibrato = false; //vibrato on or off
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
bool PLAY=true;
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

void rawHandler1();
void rawHandler2();
// void rawHandler3();
// void rawHandler4();


void pwm_interrupt_handler() {
    pwm_clear_irq(pwm_gpio_to_slice_num(AUDIO_PIN));
    if(PLAY){
        if(vibrato){
            if (wav_position < (WAV_DATA_LENGTH<<3) - 1) { 
                // set pwm level 
                // allow the pwm value to repeat for 8 cycles this is >>3 
                switch (button){
                    case 0: //sin wave
                        pwm_set_gpio_level(AUDIO_PIN, SIN_WAV_DATA[wav_position>>3]);
                        break;
                    case 1: //square wave
                        pwm_set_gpio_level(AUDIO_PIN, SQR_WAV_DATA[wav_position>>3]);
                        break;
                    case 2: //Triangle wave
                        pwm_set_gpio_level(AUDIO_PIN, TRI_WAV_DATA[wav_position>>3]);
                        break;
                    case 3: //Saw wave
                        pwm_set_gpio_level(AUDIO_PIN, SAW_WAV_DATA[wav_position>>3]);
                        break;
                    case 4: //Reverse Saw wave
                        pwm_set_gpio_level(AUDIO_PIN, R_SAW_WAV_DATA[wav_position>>3]);
                        break;
                    case 5:
                        pwm_set_gpio_level(AUDIO_PIN, PRBA_WAV_DATA[wav_position>>3]);
                        break;
                    case 6:
                        pwm_set_gpio_level(AUDIO_PIN, HARMONICS_WAV_DATA[wav_position>>3]);
                        break;
                    // case 7:
                    //     pwm_set_gpio_level(AUDIO_PIN, EVEN_HARMONICS_WAV_DATA[wav_position>>3]);
                    //     break;
                    // case 8:
                    //     pwm_set_gpio_level(AUDIO_PIN, ODD_HARMONICS_WAV_DATA[wav_position>>3]);
                    //     break;
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
                switch (button){
                    case 0: //sin wave
                        pwm_set_gpio_level(AUDIO_PIN, SIN_WAV_DATA[wav_position>>3]);
                        break;
                    case 1: //square wave
                        pwm_set_gpio_level(AUDIO_PIN, SQR_WAV_DATA[wav_position>>3]);
                        break;
                    case 2: //Triangle wave
                        pwm_set_gpio_level(AUDIO_PIN, TRI_WAV_DATA[wav_position>>3]);
                        break;
                    case 3: //Saw wave
                        pwm_set_gpio_level(AUDIO_PIN, SAW_WAV_DATA[wav_position>>3]);
                        break;
                    case 4: //Reverse Saw wave
                        pwm_set_gpio_level(AUDIO_PIN, R_SAW_WAV_DATA[wav_position>>3]);
                        break;
                    case 5:
                        pwm_set_gpio_level(AUDIO_PIN, PRBA_WAV_DATA[wav_position>>3]);
                        break;
                    case 6:
                        pwm_set_gpio_level(AUDIO_PIN, HARMONICS_WAV_DATA[wav_position>>3]);
                        break;
                    // case 7:
                    //     pwm_set_gpio_level(AUDIO_PIN, EVEN_HARMONICS_WAV_DATA[wav_position>>3]);
                    //     break;
                    // case 8:
                    //     pwm_set_gpio_level(AUDIO_PIN, ODD_HARMONICS_WAV_DATA[wav_position>>3]);
                    //     break;
                }
                wav_position++;
            } else {
                
                //frequency=WAV_FREQUENCY*adc_value;
                // reset to start
                wav_position = 0;
                
                
            }
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

    // gpio_init(VIBRATO_PIN);
    // gpio_set_dir(VIBRATO_PIN,GPIO_IN);
    // gpio_set_irq_enabled(VIBRATO_PIN,GPIO_IRQ_EDGE_RISE ,true);
    // gpio_add_raw_irq_handler_masked(( 0x01 << VIBRATO_PIN),&rawHandler1);

    gpio_init(GATE);
    gpio_set_dir(GATE,GPIO_IN);
    gpio_pull_down(GATE);
    gpio_set_irq_enabled(GATE,GPIO_IRQ_EDGE_RISE|GPIO_IRQ_EDGE_FALL,true);
    gpio_add_raw_irq_handler_masked(( 0x01 << GATE),&rawHandler1);
    
    // gpio_init(WAVEBUTTON);
    // gpio_set_dir(WAVEBUTTON,GPIO_IN);
    // gpio_set_irq_enabled(WAVEBUTTON,GPIO_IRQ_EDGE_RISE ,true);
    // gpio_add_raw_irq_handler_masked(( 0x01 << WAVEBUTTON),&rawHandler1);
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
void rawHandler1(){
    // if(gpio_get_irq_event_mask(VIBRATO_PIN) & GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL){
    //     gpio_acknowledge_irq(VIBRATO_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL);
    //     if(button>0){
    //         button--;
    //     }
    //     else{
    //         button=8;
    //     }
    // }
    // if(gpio_get_irq_event_mask(WAVEBUTTON) & GPIO_IRQ_EDGE_RISE ){
    //     gpio_acknowledge_irq(WAVEBUTTON, GPIO_IRQ_EDGE_RISE );

    //     if(button<8){
    //         button++;
    //     }
    //     else{
    //         button=0;
    //     }
    // }
    if(gpio_get_irq_event_mask(VIBRATO_PIN) & GPIO_IRQ_EDGE_RISE ){
        gpio_acknowledge_irq(VIBRATO_PIN, GPIO_IRQ_EDGE_RISE );
        PLAY=true;
        adc_value=((adc_read())*conversionfactor);
        if(adc_value>2){
            if(adc_value>2.5f){
                if(adc_value>2.75){
                    if(adc_value>2.875){
                        if(adc_value<2.9375){
                            frequency=OCTAVE3[11];
                        }else{
                            frequency=OCTAVE3[12];
                        }
                    }
                    else{
                        frequency=OCTAVE3[10];
                    }
                }
                else{
                    if(adc_value>2.625){
                        if(adc_value>2.6875){
                            frequency=OCTAVE3[9];
                        }
                        else{
                            frequency=OCTAVE3[8];
                        }
                    }
                    else{
                        if(adc_value>2.5625){
                            frequency=OCTAVE3[7];
                        }else{
                            frequency=OCTAVE3[6];
                        }
                    }
                }
            }else{
                if(adc_value>2.25){
                    if(adc_value>2.375){
                        if(adc_value>2.4375){
                            frequency=OCTAVE3[6];
                        }else{
                            frequency=OCTAVE3[5];
                        }
                    }else{
                        if(adc_value>2.3125){
                            frequency=OCTAVE3[4];
                        }
                        else{
                            frequency=OCTAVE3[3];
                        }
                    }
                }else{
                    if(adc_value>2.125){
                        if(adc_value>2.1875){
                            frequency=OCTAVE3[3];
                        }else{
                            frequency=OCTAVE3[2];
                        }
                    }else{
                        if(adc_value>2.0625){
                            frequency=OCTAVE3[1];
                        }
                        else{
                            frequency=OCTAVE3[0];
                        }
                    }
                }
            }
        }else if(adc_value>1){
            if(adc_value>1.5f){
                if(adc_value>1.75){
                    if(adc_value>1.875){
                        if(adc_value<1.9375){
                            frequency=OCTAVE2[11];
                        }else{
                            frequency=OCTAVE2[12];
                        }
                    }
                    else{
                        frequency=OCTAVE2[10];
                    }
                }
                else{
                    if(adc_value>1.625){
                        if(adc_value>1.6875){
                            frequency=OCTAVE2[9];
                        }
                        else{
                            frequency=OCTAVE2[8];
                        }
                    }
                    else{
                        if(adc_value>1.5625){
                            frequency=OCTAVE2[7];
                        }else{
                            frequency=OCTAVE2[6];
                        }
                    }
                }
            }else{
                if(adc_value>1.25){
                    if(adc_value>1.375){
                        if(adc_value>1.4375){
                            frequency=OCTAVE2[6];
                        }else{
                            frequency=OCTAVE2[5];
                        }
                    }else{
                        if(adc_value>1.3125){
                            frequency=OCTAVE2[4];
                        }
                        else{
                            frequency=OCTAVE2[3];
                        }
                    }
                }else{
                    if(adc_value>1.125){
                        if(adc_value>1.1875){
                            frequency=OCTAVE2[3];
                        }else{
                            frequency=OCTAVE2[2];
                        }
                    }else{
                        if(adc_value>1.0625){
                            frequency=OCTAVE2[1];
                        }
                        else{
                            frequency=OCTAVE2[0];
                        }
                    }
                }
            }
        }else{
            if(adc_value>0.5f){
                if(adc_value>0.75){
                    if(adc_value>0.875){
                        if(adc_value<0.9375){
                            frequency=OCTAVE1[11];
                        }else{
                            frequency=OCTAVE1[12];
                        }
                    }
                    else{
                        frequency=OCTAVE1[10];
                    }
                }
                else{
                    if(adc_value>0.625){
                        if(adc_value>0.6875){
                            frequency=OCTAVE1[9];
                        }
                        else{
                            frequency=OCTAVE1[8];
                        }
                    }
                    else{
                        if(adc_value>0.5625){
                            frequency=OCTAVE1[7];
                        }else{
                            frequency=OCTAVE1[6];
                        }
                    }
                }
            }else{
                if(adc_value>0.25){
                    if(adc_value>0.375){
                        if(adc_value>0.4375){
                            frequency=OCTAVE1[6];
                        }else{
                            frequency=OCTAVE1[5];
                        }
                    }else{
                        if(adc_value>0.3125){
                            frequency=OCTAVE1[4];
                        }
                        else{
                            frequency=OCTAVE1[3];
                        }
                    }
                }else{
                    if(adc_value>0.125){
                        if(adc_value>0.1875){
                            frequency=OCTAVE1[3];
                        }else{
                            frequency=OCTAVE1[2];
                        }
                    }else{
                        if(adc_value>0.0625){
                            frequency=OCTAVE1[1];
                        }
                        else{
                            frequency=OCTAVE1[0];
                        }
                    }
                }
            }

        }
        updateClockDiv(clockDivChange(frequency));
    }else if(gpio_get_irq_event_mask(GATE) & GPIO_IRQ_EDGE_FALL ){
        gpio_acknowledge_irq(GATE, GPIO_IRQ_EDGE_FALL );
        PLAY=false;
    }
}