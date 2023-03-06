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
float frequency=WAV_FREQUENCY;
float clkDiv=2.0f;
int Hnumber=1;

int value=0;
int cycleNum=0;
uint8_t FFT[]={
    127, 132, 136, 141, 146, 150, 155, 159, 164, 168, 173, 177, 181, 185, 189, 193, 197, 201, 205, 209, 212, 216, 219, 222, 225, 228, 231, 233, 236, 238, 240, 242, 244, 246, 248, 249, 250, 251, 252, 253, 254, 254, 254, 254, 254, 254, 254, 253, 252, 251, 250, 249, 248, 246, 244, 242, 240, 238, 236, 233, 231, 228, 225, 222, 219, 215, 212, 208, 205, 201, 197, 193, 189, 185, 181, 177, 172, 168, 163, 159, 154, 150, 145, 141, 136, 131, 127, 122, 117, 113, 108, 104, 99, 94, 90, 86, 81, 77, 73, 68, 64, 60, 56, 53, 49, 45, 42, 38, 35, 32, 29, 26, 23, 20, 18, 16, 13, 11, 10, 8, 6, 5, 4, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 8, 10, 12, 14, 16, 18, 21, 24, 26, 29, 32, 36, 39, 42, 46, 49, 53, 57, 61, 65, 69, 73, 78, 82, 86, 91, 95, 100, 104, 109, 114, 118, 123, 128
};
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
    if(gpio_get_irq_event_mask(WAVEBUTTON) & GPIO_IRQ_EDGE_RISE ){
        gpio_acknowledge_irq(WAVEBUTTON, GPIO_IRQ_EDGE_RISE );
        adc_value=(adc_read())*conversionfactor;
        frequency=WAV_FREQUENCY*adc_value;
        updateClockDiv(clockDivChange(frequency));
    }
}
void pwm_interrupt_handler() {
    pwm_clear_irq(pwm_gpio_to_slice_num(AUDIO_PIN));
        if (wav_position < (WAV_DATA_LENGTH<<3) - 1) { 
            // set pwm level 
            // allow the pwm value to repeat for 8 cycles this is >>3 
            switch (cycleNum){
                case 0: //base Harmonic
                    value=FFT[wav_position>>3];
                    Hnumber=1;
                    break;
                case 1: //+harmonic1+2
                    value=FFT[wav_position>>3]+HARMONIC2_WAV_DATA[wav_position>>3];
                    Hnumber=2;
                    break;
                case 2: //+harmonic
                    value=FFT[wav_position>>3]+HARMONIC3_WAV_DATA[wav_position>>3];
                    Hnumber=3;
                    break;
                case 3: //+harmonic
                    value=FFT[wav_position>>3]+HARMONIC4_WAV_DATA[wav_position>>3];
                    Hnumber=4;
                    break;
                case 4: //+harmonic
                    value=FFT[wav_position>>3]+HARMONIC5_WAV_DATA[wav_position>>3];
                    Hnumber=5;
                    break;
                case 5: //+harmonic
                    value=FFT[wav_position>>3]+HARMONIC6_WAV_DATA[wav_position>>3];
                    Hnumber=6;
                    break;
                case 6: //+harmonic
                    value=FFT[wav_position>>3]+HARMONIC7_WAV_DATA[wav_position>>3];
                    Hnumber=7;
                    break;
                case 7: //+harmonic
                    value=FFT[wav_position>>3]+HARMONIC8_WAV_DATA[wav_position>>3];
                    Hnumber=8;
                    break;
                case 8: //+harmonic
                    value=FFT[wav_position>>3]+HARMONIC9_WAV_DATA[wav_position>>3];
                    Hnumber=9;
                    break;
                case 9: //-harmonic
                    value=FFT[wav_position>>3]-HARMONIC8_WAV_DATA[wav_position>>3];
                    Hnumber=8;
                    break;
                case 10: //-harmonic
                    value=FFT[wav_position>>3]-HARMONIC7_WAV_DATA[wav_position>>3];
                    Hnumber=7;
                    break;
                case 11: //-harmonic
                    value=FFT[wav_position>>3]-HARMONIC6_WAV_DATA[wav_position>>3];
                    Hnumber=6;
                    break;
                case 12: //-harmonic
                    value=FFT[wav_position>>3]-HARMONIC5_WAV_DATA[wav_position>>3];
                    Hnumber=5;
                    break;
                case 13: //-harmonic
                    value=FFT[wav_position>>3]-HARMONIC4_WAV_DATA[wav_position>>3];
                    Hnumber=4;
                    break;
                case 14: //-harmonic
                    value=FFT[wav_position>>3]-HARMONIC3_WAV_DATA[wav_position>>3];
                    Hnumber=3;
                    break;
                case 15: //-harmonic
                    value=FFT[wav_position>>3]-HARMONIC2_WAV_DATA[wav_position>>3];
                    Hnumber=2;
                    break;

            }
            FFT[wav_position>>3]=value;
            pwm_set_gpio_level(AUDIO_PIN, round(value/Hnumber));
            wav_position++;
        } else {
            adc_value=(adc_read())*conversionfactor;
            frequency=WAV_FREQUENCY*adc_value;
            // reset to start
            wav_position = 0;
            updateClockDiv(clockDivChange(frequency));
            cycleNum++;
            if(cycleNum==16){
                cycleNum=0;
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
