#include <stdio.h>
#include <math.h>
#include "button.h"
#include "pico/stdlib.h"    
#include "hardware/irq.h"  
#include "hardware/pwm.h"  
#include "hardware/sync.h" 
#include "hardware/adc.h"

  
#define AUDIO_PIN 28  
#define ADC_PIN 26


#define HM_ODD_DOWN 13
#define HM_ODD_UP 12
#define HM_EVEN_DOWN 11
#define HM_EVEN_UP 10

#define PORABOLA 9
#define SAWTOOTH 8
#define TRIANGLE 6
#define SQUARE 5
#define SINE 2
#define VIBRATO_PIN 0
// #define GATE 15

#include "waves.h"
void rawHandler1();
int wav_position = 0;
float adc_value=0;
const float conversionfactor=3.3f/(1<<12);
bool vibrato = false; //vibrato on or off
float frequency=WAV_FREQUENCY;
float currentF = WAV_FREQUENCY;
int freqNum=0;
int interval=0;
//int FreqCount = round(sizeof(frequencies)/sizeof(frequencies[0]))-1;
float clkDiv=2.0f;
const float vibsize = WAV_FREQUENCY/12;
float vibchangeParam = vibsize/12;
int buttonNum = 0;
int evenHarmonics=0;
int oddHarmonics=0;
int value=0;
bool vibUP=true;
int subScript = 0;

closure_t handlers[28] = {NULL};
alarm_id_t alarm_ids[28];
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
long long int handle_button_alarm(long int a, void *p) {
  button_t *b = (button_t *)(p);
  bool state = gpio_get(b->pin);
  if (state != b->state) {
    b->state = state;
    b->onchange(b);
  }
  return 0;
}

void handle_button_interrupt(void *p) {
  button_t *b = (button_t *)(p);
  if (alarm_ids[b->pin]) cancel_alarm(alarm_ids[b->pin]);
  alarm_ids[b->pin] = add_alarm_in_us(DEBOUNCE_US, handle_button_alarm, b, true);
}

void handle_interrupt(uint gpio, uint32_t events) {
  closure_t handler = handlers[gpio];
  handler.fn(handler.argument);
}

void listen(uint pin, int condition, handler fn, void *arg) {
  gpio_set_irq_enabled_with_callback(pin, condition, true, handle_interrupt);
  closure_t *handler = malloc(sizeof(closure_t));
  handler->argument = arg;
  handler->fn = fn;
  handlers[pin] = *handler;
}

button_t * create_button(int pin, void (*onchange)(button_t *)) {
  gpio_init(pin);
  gpio_pull_up(pin);
  button_t *b = (button_t *)(malloc(sizeof(button_t)));
  listen(pin, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, handle_button_interrupt, b);
  b->pin = pin;
  b->onchange = onchange;
  b->state = gpio_get(pin);
  return b;
}
void onchange(button_t *button_p) {
  button_t *button = (button_t*)button_p;

  if(button->state) return; // Ignore button release. Invert the logic if using
                            // a pullup (internal or external).

  switch(button->pin){
    case SINE:
        buttonNum=0;
        break;
    case SQUARE:
        buttonNum=1;
        break;
    case TRIANGLE:
        buttonNum=2;
        break;
    case SAWTOOTH:
        if(buttonNum==3){
            buttonNum=4;
        }else{
            buttonNum=3;
        }
        break;
    case PORABOLA:
        buttonNum=5;
        break;
    case HM_EVEN_UP:
        if(evenHarmonics<6){
            evenHarmonics++;
        }
        
        break;
    case HM_EVEN_DOWN:
        if(evenHarmonics!=0){
            evenHarmonics--;
        }
        break;
    case HM_ODD_UP:
        if(oddHarmonics<5){
            oddHarmonics++;
        }
        break;
    case HM_ODD_DOWN:
        if(oddHarmonics!=0){
            oddHarmonics--;
        }
        break;
    case VIBRATO_PIN:
        if(vibrato){
            vibrato=false;
        }else{
            vibrato=true;
        }
        break;
  }
}


void pwm_interrupt_handler() {
    pwm_clear_irq(pwm_gpio_to_slice_num(AUDIO_PIN));
    
        if(vibrato){
            if (wav_position < (WAV_DATA_LENGTH<<3) - 1) { 
                // set pwm level 
                // allow the pwm value to repeat for 8 cycles this is >>3 
                switch (buttonNum){
                    case 0: //sin wave
                        value=SIN_WAV_DATA[wav_position>>3];
                        break;
                    case 1: //square wave
                        value=SQR_WAV_DATA[wav_position>>3];
                        break;
                    case 2: //Triangle wave
                        value=TRI_WAV_DATA[wav_position>>3];
                        break;
                    case 3: //Saw wave
                        value=SAW_WAV_DATA[wav_position>>3];
                        break;
                    case 4: //Reverse Saw wave
                        value=R_SAW_WAV_DATA[wav_position>>3];
                        break;
                    case 5:
                        value=PRBA_WAV_DATA[wav_position>>3];
                        break;
                }
                switch(evenHarmonics){
                    case 0:
                        break;
                    case 1:
                        value=value+HARMONIC2_WAV_DATA[wav_position>>3];
                        break;
                    case 2:
                        value=value+HARMONIC2_WAV_DATA[wav_position>>3]+HARMONIC4_WAV_DATA[wav_position>>3];
                        break;
                    case 3:
                        value=value+HARMONIC2_WAV_DATA[wav_position>>3]+HARMONIC4_WAV_DATA[wav_position>>3]+HARMONIC6_WAV_DATA[wav_position>>3];
                        break;
                    case 4:
                        value=value+HARMONIC2_WAV_DATA[wav_position>>3]+HARMONIC4_WAV_DATA[wav_position>>3]+HARMONIC6_WAV_DATA[wav_position>>3]+HARMONIC8_WAV_DATA[wav_position>>3];
                        break;
                    case 5:
                        value=value+HARMONIC2_WAV_DATA[wav_position>>3]+HARMONIC4_WAV_DATA[wav_position>>3]+HARMONIC6_WAV_DATA[wav_position>>3]+HARMONIC8_WAV_DATA[wav_position>>3]+HARMONIC10_WAV_DATA[wav_position>>3];
                        break;
                    case 6:
                        value=value+HARMONIC2_WAV_DATA[wav_position>>3]+HARMONIC4_WAV_DATA[wav_position>>3]+HARMONIC6_WAV_DATA[wav_position>>3]+HARMONIC8_WAV_DATA[wav_position>>3]+HARMONIC10_WAV_DATA[wav_position>>3]+HARMONIC12_WAV_DATA[wav_position>>3];
                        break;
                }
                switch(oddHarmonics){
                    case 0:
                        break;
                    case 1:
                        value=value+HARMONIC3_WAV_DATA[wav_position>>3];
                        break;
                    case 2:
                        value=value+HARMONIC3_WAV_DATA[wav_position>>3]+HARMONIC5_WAV_DATA[wav_position>>3];
                        break;
                    case 3:
                        value=value+HARMONIC3_WAV_DATA[wav_position>>3]+HARMONIC5_WAV_DATA[wav_position>>3]+HARMONIC7_WAV_DATA[wav_position>>3];
                        break;
                    case 4:
                        value=value+HARMONIC3_WAV_DATA[wav_position>>3]+HARMONIC5_WAV_DATA[wav_position>>3]+HARMONIC7_WAV_DATA[wav_position>>3]+HARMONIC9_WAV_DATA[wav_position>>3];
                        break;
                    case 5:
                        value=value+HARMONIC3_WAV_DATA[wav_position>>3]+HARMONIC5_WAV_DATA[wav_position>>3]+HARMONIC7_WAV_DATA[wav_position>>3]+HARMONIC9_WAV_DATA[wav_position>>3]+HARMONIC11_WAV_DATA[wav_position>>3];
                        break;
                }
                pwm_set_gpio_level(AUDIO_PIN, round(value/(evenHarmonics+oddHarmonics+1)));
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
                switch (buttonNum){
                    case 0: //sin wave
                        value=SIN_WAV_DATA[wav_position>>3];
                        break;
                    case 1: //square wave
                        value=SQR_WAV_DATA[wav_position>>3];
                        break;
                    case 2: //Triangle wave
                        value=TRI_WAV_DATA[wav_position>>3];
                        break;
                    case 3: //Saw wave
                        value=SAW_WAV_DATA[wav_position>>3];
                        break;
                    case 4: //Reverse Saw wave
                        value=R_SAW_WAV_DATA[wav_position>>3];
                        break;
                    case 5:
                        value=PRBA_WAV_DATA[wav_position>>3];
                        break;
                }
                switch(evenHarmonics){
                    case 0:
                        break;
                    case 1:
                        value=value+HARMONIC2_WAV_DATA[wav_position>>3];
                        break;
                    case 2:
                        value=value+HARMONIC2_WAV_DATA[wav_position>>3]+HARMONIC4_WAV_DATA[wav_position>>3];
                        break;
                    case 3:
                        value=value+HARMONIC2_WAV_DATA[wav_position>>3]+HARMONIC4_WAV_DATA[wav_position>>3]+HARMONIC6_WAV_DATA[wav_position>>3];
                        break;
                    case 4:
                        value=value+HARMONIC2_WAV_DATA[wav_position>>3]+HARMONIC4_WAV_DATA[wav_position>>3]+HARMONIC6_WAV_DATA[wav_position>>3]+HARMONIC8_WAV_DATA[wav_position>>3];
                        break;
                    case 5:
                        value=value+HARMONIC2_WAV_DATA[wav_position>>3]+HARMONIC4_WAV_DATA[wav_position>>3]+HARMONIC6_WAV_DATA[wav_position>>3]+HARMONIC8_WAV_DATA[wav_position>>3]+HARMONIC10_WAV_DATA[wav_position>>3];
                        break;
                    case 6:
                        value=value+HARMONIC2_WAV_DATA[wav_position>>3]+HARMONIC4_WAV_DATA[wav_position>>3]+HARMONIC6_WAV_DATA[wav_position>>3]+HARMONIC8_WAV_DATA[wav_position>>3]+HARMONIC10_WAV_DATA[wav_position>>3]+HARMONIC12_WAV_DATA[wav_position>>3];
                        break;
                }
                switch(oddHarmonics){
                    case 0:
                        break;
                    case 1:
                        value=value+HARMONIC3_WAV_DATA[wav_position>>3];
                        break;
                    case 2:
                        value=value+HARMONIC3_WAV_DATA[wav_position>>3]+HARMONIC5_WAV_DATA[wav_position>>3];
                        break;
                    case 3:
                        value=value+HARMONIC3_WAV_DATA[wav_position>>3]+HARMONIC5_WAV_DATA[wav_position>>3]+HARMONIC7_WAV_DATA[wav_position>>3];
                        break;
                    case 4:
                        value=value+HARMONIC3_WAV_DATA[wav_position>>3]+HARMONIC5_WAV_DATA[wav_position>>3]+HARMONIC7_WAV_DATA[wav_position>>3]+HARMONIC9_WAV_DATA[wav_position>>3];
                        break;
                    case 5:
                        value=value+HARMONIC3_WAV_DATA[wav_position>>3]+HARMONIC5_WAV_DATA[wav_position>>3]+HARMONIC7_WAV_DATA[wav_position>>3]+HARMONIC9_WAV_DATA[wav_position>>3]+HARMONIC11_WAV_DATA[wav_position>>3];
                        break;
                }
                pwm_set_gpio_level(AUDIO_PIN, round(value/(evenHarmonics+oddHarmonics+1)));
                wav_position++;
            } else {
                adc_value=((adc_read())*conversionfactor);
                subScript=round(60*adc_value/3);
                frequency=OCTAVE1[subScript];

                updateClockDiv(clockDivChange(frequency));
                // reset to start
                wav_position = 0;
                
                
            }
        }
    //}
    
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

    // gpio_init(GATE);
    // gpio_set_dir(GATE,GPIO_IN);
    // gpio_pull_down(GATE);
    // gpio_set_irq_enabled(GATE,GPIO_IRQ_EDGE_RISE|GPIO_IRQ_EDGE_FALL,true);
    // gpio_add_raw_irq_handler_masked(( 0x01 << GATE),&rawHandler1);

    button_t *sine = create_button(SINE, onchange);
    button_t *square = create_button(SQUARE, onchange);
    button_t *triangle = create_button(TRIANGLE, onchange);
    button_t *porabola = create_button(PORABOLA, onchange);
    button_t *hm_even_up = create_button(HM_EVEN_UP, onchange);
    button_t *hm_even_down = create_button(HM_EVEN_DOWN, onchange);
    button_t *hm_odd_up = create_button(HM_ODD_UP, onchange);
    button_t *hm_odd_down = create_button(HM_ODD_DOWN, onchange);
    button_t *vibrato = create_button(VIBRATO_PIN, onchange);
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
