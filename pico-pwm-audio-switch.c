#include <stdio.h>
#include "pico/stdlib.h"   // stdlib 
#include "hardware/irq.h"  // interrupts
#include "hardware/pwm.h"  // pwm 
#include "hardware/sync.h" // wait for interrupt 
#include "./piano-C4.h"
#include "./piano-C5.h"
#include "./piano-C6.h"
#include "./piano-G3.h"
#include "./piano-G4.h"
#include "./piano-G5.h"
#include "./piano-G6.h"
// Audio PIN is to match some of the design guide shields. 
#define AUDIO_PIN 28  // you can change this to whatever you like
#define *WAV_DATA_LENGTH[]={&C4_WAV_DATA_LENGTH,&C5_WAV_DATA_LENGTH,&C6_WAV_DATA_LENGTH,&G3_WAV_DATA_LENGTH,&G4_WAV_DATA_LENGTH,&G5_WAV_DATA_LENGTH,&G6_WAV_DATA_LENGTH};

uint8_t *WAV_DATA[]={&C4_WAV_DATA,&C5_WAV_DATA,&C6_WAV_DATA,&G3_WAV_DATA,&G4_WAV_DATA,&G5_WAV_DATA,&G6_WAV_DATA};

int wav_position = 0;
int i = 0
/*
 * PWM Interrupt Handler which outputs PWM level and advances the 
 * current sample. 
 * 
 * We repeat the same value for 8 cycles this means sample rate etc
 * adjust by factor of 8   (this is what bitshifting <<3 is doing)
 * 
 */
void pwm_interrupt_handler() {
    pwm_clear_irq(pwm_gpio_to_slice_num(AUDIO_PIN));    
    if (wav_position < (*WAV_DATA_LENGTH[i]<<3) - 1) { 
        // set pwm level 
        // allow the pwm value to repeat for 8 cycles this is >>3 
        pwm_set_gpio_level(AUDIO_PIN, *WAV_DATA[i][wav_position>>3]);  
        wav_position++;
    } else {
        // reset to start
        wav_position = 0;
        if(i<sizeof(WAV_DATA)){
            i+=1
        }
        else{
            i=0
        }
    }
}

int main(void) {
    /* Overclocking for fun but then also so the system clock is a 
     * multiple of typical audio sampling rates.
     */
    stdio_init_all();
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
    /* Base clock 176,000,000 Hz divide by wrap 250 then the clock divider further divides
     * to set the interrupt rate. 
     * 
     * 11 KHz is fine for speech. Phone lines generally sample at 8 KHz
     * 
     * 
     * So clkdiv should be as follows for given sample rate
     *  8.0f for 11 KHz
     *  4.0f for 22 KHz
     *  2.0f for 44 KHz etc
     */
    pwm_config_set_clkdiv(&config, 8.0f); 
    pwm_config_set_wrap(&config, 250); 
    pwm_init(audio_pin_slice, &config, true);

    pwm_set_gpio_level(AUDIO_PIN, 0);

    while(1) {
        __wfi(); // Wait for Interrupt
    }
}
