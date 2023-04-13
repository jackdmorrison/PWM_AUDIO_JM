#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"   // stdlib
#include "hardware/irq.h"  // interrupts
#include "hardware/pwm.h"  // pwm
#include "hardware/sync.h" // wait for interrupt
#include "hardware/adc.h"

// Audio PIN is to match some of the design guide shields.
#define AUDIO_PIN 28 // you can change this to whatever you like
#define ADC_PIN 26
#define PULSE_PIN 15

// #include "waves.h"
int wav_position = 0;
float adc_value = 0;
const float conversionfactor=1.65f/(1<<12);
//float frequency=WAV_FREQUENCY;
float clkDiv=2.0f;
int pulseLength=86;
int wavelength=172;
bool pulseMode=true;
float tan_theta=1/86;
float n_tan_theta=86;
int value=0;
int var=0;
void updateClockDiv(float clkDiv)
{
    int pin_slice = pwm_gpio_to_slice_num(AUDIO_PIN);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, clkDiv);
    pwm_config_set_wrap(&config, 250);
    pwm_init(pin_slice, &config, true);
    pwm_set_gpio_level(AUDIO_PIN, 0);
}

void pwm_interrupt_handler()
{
    pwm_clear_irq(pwm_gpio_to_slice_num(AUDIO_PIN));
    var=wav_position>>3;
    if (wav_position<(wavelength<<3) - 1 ) { 
        // set pwm level 

        value=round(255*(1+((var-wavelength/2)/wavelength/2)*((var-wavelength/2)/wavelength/2)));
         

         
        pwm_set_gpio_level(AUDIO_PIN,value);
        wav_position++;

    }
    else
    {
        adc_value = (adc_read()) * conversionfactor;
        wavelength = round(172 * adc_value);
        // reset to start
        wav_position = 0;
    }
}

int main(void)
{
    /* Overclocking for fun but then also so the system clock is a
     * multiple of typical audio sampling rates.
     */
    stdio_init_all();
    adc_init();
    adc_gpio_init(ADC_PIN);
    adc_select_input(0);

    // gpio_init(PULSE_PIN);
    // gpio_set_dir(PULSE_PIN,GPIO_IN);
    // gpio_set_irq_enabled(PULSE_PIN,GPIO_IRQ_EDGE_RISE ,true);
    // gpio_add_raw_irq_handler(PULSE_PIN, rawHandler);
    // irq_set_enabled(IO_IRQ_BANK0, true);

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

    while (1)
    {
        __wfi(); // Wait for Interrupt
    }
}
