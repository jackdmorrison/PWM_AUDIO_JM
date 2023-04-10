#include <stdio.h>
#include <math.h>
#include "button.h"
#include <pico/stdlib.h>    
#include "hardware/irq.h"  
#include "hardware/pwm.h"  
#include "hardware/sync.h" 
#include "hardware/adc.h"
#include "hardware/vreg.h"

  
#define AUDIO_PIN  16
#define AUDIO_PIN2 18
#define ADC_PIN 28
#define ADC_PIN2 27


#define HM_ODD_DOWN 12
#define HM_ODD_UP 11
#define HM_EVEN_DOWN 10
#define HM_EVEN_UP 9

#define PORABOLA 8
#define SAWTOOTH 7
#define TRIANGLE 5

#define SQUARE 4
#define SINE 2
#define SWITCHSIGNAL 13
#define VIBRATO_PIN 0
#define GATE 20
#define GATE2 21

#include "waves.h"
pwm_config config;
uint audio_pin_slice;
uint audio_pin_slice2;
uint audio_pin_channel;
uint audio_pin_channel2;

void rawHandler1();
int wav_position = 0;
int wav_position2=0;
float adc_value=0;
const float conversionfactor=3.3f/(1<<12);

bool vibrato = false; //vibrato on or off
bool vibrato2 = false; //vibrato on or off

int subScript = 0;
int subScript2=0;

float frequency=freqList[0];
float frequency2=freqList[0];

float currentF = freqList[0];
float currentF2 = freqList[0];

float upperVibrato=freqList[1];
float upperVibrato2=freqList[1];

float lowerVibrato=lowestFrequency;
float lowerVibrato2=lowestFrequency;

float vibchangeParam = (freqList[1]-lowestFrequency)/24;
float vibchangeParam2 = (freqList[1]-lowestFrequency)/24;

int buttonNum = 0;
int buttonNum2 = 0;

int evenHarmonics=0;
int evenHarmonics2=0;

int oddHarmonics=0;
int oddHarmonics2=0;

int value=0;
int value2=0;

bool vibUP=true;
bool vibUP2=true;
bool signal1=true;
bool PLAY=false;
bool PLAY2=false;
closure_t handlers[28] = {NULL};
alarm_id_t alarm_ids[28];
double sine_wave_y(double x) {
    return sin(x);
}

float clockDivChange( float newFrequency){
    if(newFrequency>freqList[42]){
        return (WAV_FREQUENCY/newFrequency)*clkDiv*4;
    }
    else if(newFrequency>freqList[36]){
        return (WAV_FREQUENCY/newFrequency)*clkDiv*2;
    }else if(newFrequency<freqList[12]){
        return (WAV_FREQUENCY/newFrequency)*clkDiv/4;
    }else if(newFrequency<freqList[24]){
        return (WAV_FREQUENCY/newFrequency)*clkDiv/2;
    }else{
        return (WAV_FREQUENCY/newFrequency)*clkDiv;
    }
    
}
void updateClockDiv(float clkDiv, int PIN,int pin_slice){
    if(1.f<=clkDiv<=256.f){
        pwm_set_clkdiv(pin_slice, clkDiv); 
        pwm_set_gpio_level(PIN, 0);
    }else if(clkDiv<1.f){
        pwm_set_clkdiv(pin_slice, 1.f); 
        pwm_set_gpio_level(PIN, 0);
    }else if(clkDiv>256.f){
        pwm_set_clkdiv(pin_slice, 256.f); 
        pwm_set_gpio_level(PIN, 0);
    }
    
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
float findValue(int buttonNumber,int evenHarmonicsNum,int oddHarmonicsNum,int wave_position){
    float value=0;
    switch (buttonNumber){
        case 0: //sin wave
            value=SIN_WAV_DATA[wave_position];
            break;
        case 1: //square wave
            value=SQR_WAV_DATA[wave_position];
            break;
        case 2: //Triangle wave
            value=TRI_WAV_DATA[wave_position];
            break;
        case 3: //Saw wave
            value=SAW_WAV_DATA[wave_position];
            break;
        case 4: //Reverse Saw wave
            value=R_SAW_WAV_DATA[wave_position];
            break;
        case 5:
            value=PRBA_WAV_DATA[wave_position];
            break;
    }
    switch(evenHarmonicsNum){
        case 0:
            break;
        case 1:
            value=value+HARMONIC2_WAV_DATA[wave_position];
            break;
        case 2:
            value=value+HARMONIC2_WAV_DATA[wave_position]+HARMONIC4_WAV_DATA[wave_position];
            break;
        case 3:
            value=value+HARMONIC2_WAV_DATA[wave_position]+HARMONIC4_WAV_DATA[wave_position]+HARMONIC6_WAV_DATA[wave_position];
            break;
        case 4:
            value=value+HARMONIC2_WAV_DATA[wave_position]+HARMONIC4_WAV_DATA[wave_position]+HARMONIC6_WAV_DATA[wave_position]+HARMONIC8_WAV_DATA[wave_position];
            break;
        case 5:
            value=value+HARMONIC2_WAV_DATA[wave_position]+HARMONIC4_WAV_DATA[wave_position]+HARMONIC6_WAV_DATA[wave_position]+HARMONIC8_WAV_DATA[wave_position]+HARMONIC10_WAV_DATA[wave_position];
            break;
        case 6:
            value=value+HARMONIC2_WAV_DATA[wave_position]+HARMONIC4_WAV_DATA[wave_position]+HARMONIC6_WAV_DATA[wave_position]+HARMONIC8_WAV_DATA[wave_position]+HARMONIC10_WAV_DATA[wave_position]+HARMONIC12_WAV_DATA[wave_position];
            break;
    }
    switch(oddHarmonicsNum){
        case 0:
            break;
        case 1:
            value=value+HARMONIC3_WAV_DATA[wave_position];
            break;
        case 2:
            value=value+HARMONIC3_WAV_DATA[wave_position]+HARMONIC5_WAV_DATA[wave_position];
            break;
        case 3:
            value=value+HARMONIC3_WAV_DATA[wave_position]+HARMONIC5_WAV_DATA[wave_position]+HARMONIC7_WAV_DATA[wave_position];
            break;
        case 4:
            value=value+HARMONIC3_WAV_DATA[wave_position]+HARMONIC5_WAV_DATA[wave_position]+HARMONIC7_WAV_DATA[wave_position]+HARMONIC9_WAV_DATA[wave_position];
            break;
        case 5:
            value=value+HARMONIC3_WAV_DATA[wave_position]+HARMONIC5_WAV_DATA[wave_position]+HARMONIC7_WAV_DATA[wave_position]+HARMONIC9_WAV_DATA[wave_position]+HARMONIC11_WAV_DATA[wave_position];
            break;
    }
    return value;
}
void onchange(button_t *button_p) {
  button_t *button = (button_t*)button_p;

  if(button->state) return; // Ignore button release. Invert the logic if using
                            // a pullup (internal or external).

  switch(button->pin){
    case SINE:
        if(signal1){
            buttonNum=0;
        }else{
            buttonNum2=0;
        }
        break;
    case SQUARE:
        if(signal1){
            buttonNum=1;
        }else{
            buttonNum2=1;
        }
        break;
    case TRIANGLE:
        if(signal1){
            buttonNum=2;
        }else{
            buttonNum2=2;
        }
        break;
    case SAWTOOTH:
        if(signal1){
            if(buttonNum==3){
                buttonNum=4;
            }else{
                buttonNum=3;
            }
        }else{
            if(buttonNum2==3){
                buttonNum2=4;
            }else{
                buttonNum2=3;
            }
        }
        break;
    case PORABOLA:
        if(signal1){
            buttonNum=5;
        }else{
            buttonNum2=5;
        }
        break;
    case HM_EVEN_UP:
        if(signal1){
            if(evenHarmonics<6){
                evenHarmonics++;
            }
        }else{
            if(evenHarmonics2<6){
                evenHarmonics2++;
            }
        }    
        break;
    case HM_EVEN_DOWN:
        if(signal1){
            if(evenHarmonics!=0){
                evenHarmonics--;
            }
        }else{
            if(evenHarmonics2!=0){
                evenHarmonics2--;
            }
        }
        break;
    case HM_ODD_UP:
        if(signal1){
            if(oddHarmonics<5){
                oddHarmonics++;
            }
        }else{
            if(oddHarmonics2<5){
                oddHarmonics2++;
            }
        }
        break;
    case HM_ODD_DOWN:
        if(signal1){
            if(oddHarmonics!=0){
                oddHarmonics--;
            }
            break;
        }else{
            if(oddHarmonics2!=0){
                oddHarmonics2--;
            }
            break;
        }
    case VIBRATO_PIN:
        if(signal1){
            if(vibrato){
                vibrato=false;
            }else{
                vibrato=true;
            }
        }else{
            if(vibrato2){
                vibrato2=false;
            }else{
                vibrato2=true;
            }
        }
        
        break;
    case SWITCHSIGNAL:
        if(signal1){
            signal1=false;
        }else{
            signal1=true;
        }
        break;
  }
}


void pwm_interrupt_handler() {
    int irq;
    irq= pwm_get_irq_status_mask();
    if(irq & (1<<0)){
    //if(gpio_get_irq_event_mask(AUDIO_PIN)){
        pwm_clear_irq(0);
        if(PLAY){
            if(frequency>freqList[48]){
                if(vibrato){
                    if (wav_position < (WAV_DATA_LENGTH) - 1) { 
                        // set pwm level 
                        // allow the pwm value to repeat for 1 cycles this is  
                        
                        pwm_set_chan_level(audio_pin_slice,audio_pin_channel, round(findValue(buttonNum,evenHarmonics,oddHarmonics,wav_position)/(evenHarmonics+oddHarmonics+1)));
                        wav_position++;
                    } else {
                        // reset to start
                        wav_position = 0;
                        if(vibUP){
                            if(currentF<upperVibrato){
                                currentF+=vibchangeParam;
                            } else{
                                vibUP=false;
                            }
                            updateClockDiv(clockDivChange(currentF),AUDIO_PIN,audio_pin_slice);
                        } else{
                            if(currentF>lowerVibrato){
                                currentF-=vibchangeParam;
                            } else{
                                vibUP=true;
                            }
                            updateClockDiv(clockDivChange(currentF),AUDIO_PIN,audio_pin_slice);
                        }
                        
                    }
                }else{
                    if (wav_position < (WAV_DATA_LENGTH) - 1) { 
                        // set pwm level 
                        // allow the pwm value to repeat for 1 cycles this is  
                        pwm_set_chan_level(audio_pin_slice,audio_pin_channel, round(findValue(buttonNum,evenHarmonics,oddHarmonics,wav_position)/(evenHarmonics+oddHarmonics+1)));
                        wav_position++;
                    } else {
                        
                        // reset to start
                        wav_position = 0;
                        
                        
                    }
                }
            }
            else if(frequency>freqList[36]){
                if(vibrato){
                    if (wav_position < (WAV_DATA_LENGTH<<1) - 1) { 
                        // set pwm level 
                        // allow the pwm value to repeat for 2 cycles this is  
                        
                        pwm_set_chan_level(audio_pin_slice,audio_pin_channel, round(findValue(buttonNum,evenHarmonics,oddHarmonics,wav_position>>1)/(evenHarmonics+oddHarmonics+1)));
                        wav_position++;
                    } else {
                        // reset to start
                        wav_position = 0;
                        if(vibUP){
                            if(currentF<upperVibrato){
                                currentF+=vibchangeParam;
                            } else{
                                vibUP=false;
                            }
                            updateClockDiv(clockDivChange(currentF),AUDIO_PIN,audio_pin_slice);
                        } else{
                            if(currentF>lowerVibrato){
                                currentF-=vibchangeParam;
                            } else{
                                vibUP=true;
                            }
                            updateClockDiv(clockDivChange(currentF),AUDIO_PIN,audio_pin_slice);
                        }
                        
                    }
                }else{
                    if (wav_position < (WAV_DATA_LENGTH<<1) - 1) { 
                        // set pwm level 
                        // allow the pwm value to repeat for 2 cycles this is  
                        pwm_set_chan_level(audio_pin_slice,audio_pin_channel, round(findValue(buttonNum,evenHarmonics,oddHarmonics,wav_position>>1)/(evenHarmonics+oddHarmonics+1)));
                        wav_position++;
                    } else {
                        
                        // reset to start
                        wav_position = 0;
                        
                        
                    }
                }
        }else if(frequency<freqList[12]){
                if(vibrato){
                    if (wav_position < (WAV_DATA_LENGTH<<4) - 1) { 
                        // set pwm level 
                        // allow the pwm value to repeat for 16 cycles this is  
                        
                        pwm_set_chan_level(audio_pin_slice,audio_pin_channel, round(findValue(buttonNum,evenHarmonics,oddHarmonics,wav_position>>4)/(evenHarmonics+oddHarmonics+1)));
                        wav_position++;
                    } else {
                        // reset to start
                        wav_position = 0;
                        if(vibUP){
                            if(currentF<upperVibrato){
                                currentF+=vibchangeParam;
                            } else{
                                vibUP=false;
                            }
                            updateClockDiv(clockDivChange(currentF),AUDIO_PIN,audio_pin_slice);
                        } else{
                            if(currentF>lowerVibrato){
                                currentF-=vibchangeParam;
                            } else{
                                vibUP=true;
                            }
                            updateClockDiv(clockDivChange(currentF),AUDIO_PIN,audio_pin_slice);
                        }
                        
                    }
                }else{
                    if (wav_position < (WAV_DATA_LENGTH<<4) - 1) { 
                        // set pwm level 
                        // allow the pwm value to repeat for 16 cycles this is  
                        pwm_set_chan_level(audio_pin_slice,audio_pin_channel, round(findValue(buttonNum,evenHarmonics,oddHarmonics,wav_position>>4)/(evenHarmonics+oddHarmonics+1)));
                        wav_position++;
                    } else {
                        
                        // reset to start
                        wav_position = 0;
                        
                        
                    }
                }
        }else if(frequency<freqList[24]){
                if(vibrato){
                    if (wav_position < (WAV_DATA_LENGTH<<3) - 1) { 
                        // set pwm level 
                        // allow the pwm value to repeat for 8 cycles this is  
                        
                        pwm_set_chan_level(audio_pin_slice,audio_pin_channel, round(findValue(buttonNum,evenHarmonics,oddHarmonics,wav_position>>3)/(evenHarmonics+oddHarmonics+1)));
                        wav_position++;
                    } else {
                        // reset to start
                        wav_position = 0;
                        if(vibUP){
                            if(currentF<upperVibrato){
                                currentF+=vibchangeParam;
                            } else{
                                vibUP=false;
                            }
                            updateClockDiv(clockDivChange(currentF),AUDIO_PIN,audio_pin_slice);
                        } else{
                            if(currentF>lowerVibrato){
                                currentF-=vibchangeParam;
                            } else{
                                vibUP=true;
                            }
                            updateClockDiv(clockDivChange(currentF),AUDIO_PIN,audio_pin_slice);
                        }
                        
                    }
                }else{
                    if (wav_position < (WAV_DATA_LENGTH<<3) - 1) { 
                        // set pwm level 
                        // allow the pwm value to repeat for 8 cycles this is  
                        pwm_set_chan_level(audio_pin_slice,audio_pin_channel, round(findValue(buttonNum,evenHarmonics,oddHarmonics,wav_position>>3)/(evenHarmonics+oddHarmonics+1)));
                        wav_position++;
                    } else {
                        
                        // reset to start
                        wav_position = 0;
                        
                        
                    }
                }
            }else{
                if(vibrato){
                    if (wav_position < (WAV_DATA_LENGTH<<2) - 1) { 
                        // set pwm level 
                        // allow the pwm value to repeat for 4 cycles this is  
                        
                        pwm_set_chan_level(audio_pin_slice,audio_pin_channel, round(findValue(buttonNum,evenHarmonics,oddHarmonics,wav_position>>2)/(evenHarmonics+oddHarmonics+1)));
                        wav_position++;
                    } else {
                        // reset to start
                        wav_position = 0;
                        if(vibUP){
                            if(currentF<upperVibrato){
                                currentF+=vibchangeParam;
                            } else{
                                vibUP=false;
                            }
                            updateClockDiv(clockDivChange(currentF),AUDIO_PIN,audio_pin_slice);
                        } else{
                            if(currentF>lowerVibrato){
                                currentF-=vibchangeParam;
                            } else{
                                vibUP=true;
                            }
                            updateClockDiv(clockDivChange(currentF),AUDIO_PIN,audio_pin_slice);
                        }
                        
                    }
                }else{
                    if (wav_position < (WAV_DATA_LENGTH<<2) - 1) { 
                        // set pwm level 
                        // allow the pwm value to repeat for 4 cycles this is  
                        pwm_set_chan_level(audio_pin_slice,audio_pin_channel, round(findValue(buttonNum,evenHarmonics,oddHarmonics,wav_position>>2)/(evenHarmonics+oddHarmonics+1)));
                        wav_position++;
                    } else {
                        
                        // reset to start
                        wav_position = 0;
                        
                        
                    }
                }
            }
            
        }
    }else if(irq & (1<<1)){
        pwm_clear_irq(1);
        if(PLAY2){
            if(frequency2>freqList[42]){
                 if(vibrato2){
                    if (wav_position2 < (WAV_DATA_LENGTH) - 1) { 
                        // set pwm level 
                        // allow the pwm value to repeat for 8 cycles this is  
                        pwm_set_chan_level(audio_pin_slice2,audio_pin_channel2, round(findValue(buttonNum2,evenHarmonics2,oddHarmonics2,wav_position2)/(evenHarmonics2+oddHarmonics2+1)));
                        wav_position2++;
                    } else {
                        // reset to start
                        wav_position2 = 0;
                        if(vibUP2){
                            if(currentF2<upperVibrato2){
                                currentF2+=vibchangeParam2;
                            } else{
                                vibUP2=false;
                            }
                            updateClockDiv(clockDivChange(currentF2),AUDIO_PIN2,audio_pin_slice2);
                        } else{
                            if(currentF2>lowerVibrato2){
                                currentF2-=vibchangeParam2;
                            } else{
                                vibUP2=true;
                            }
                            updateClockDiv(clockDivChange(currentF2),AUDIO_PIN2,audio_pin_slice2);
                        }
                        
                    }
                }else{
                    if (wav_position2 < (WAV_DATA_LENGTH) - 1) { 
                        // set pwm level 
                        // allow the pwm value to repeat for 8 cycles this is  
                        pwm_set_chan_level(audio_pin_slice2,audio_pin_channel2, round(findValue(buttonNum2,evenHarmonics2,oddHarmonics2,wav_position2)/(evenHarmonics2+oddHarmonics2+1)));
                        wav_position2++;
                    } else {
                        
                        // reset to start
                        wav_position2 = 0;
                        
                        
                    }
                }
            }else if(frequency2>freqList[36]){
                if(vibrato2){
                    if (wav_position2 < (WAV_DATA_LENGTH<<1) - 1) { 
                        // set pwm level 
                        // allow the pwm value to repeat for 8 cycles this is  
                        pwm_set_chan_level(audio_pin_slice2,audio_pin_channel2, round(findValue(buttonNum2,evenHarmonics2,oddHarmonics2,wav_position2>>1)/(evenHarmonics2+oddHarmonics2+1)));
                        wav_position2++;
                    } else {
                        // reset to start
                        wav_position2 = 0;
                        if(vibUP2){
                            if(currentF2<upperVibrato2){
                                currentF2+=vibchangeParam2;
                            } else{
                                vibUP2=false;
                            }
                            updateClockDiv(clockDivChange(currentF2),AUDIO_PIN2,audio_pin_slice2);
                        } else{
                            if(currentF2>lowerVibrato2){
                                currentF2-=vibchangeParam2;
                            } else{
                                vibUP2=true;
                            }
                            updateClockDiv(clockDivChange(currentF2),AUDIO_PIN2,audio_pin_slice2);
                        }
                        
                    }
                }else{
                    if (wav_position2 < (WAV_DATA_LENGTH<<1) - 1) { 
                        // set pwm level 
                        // allow the pwm value to repeat for 8 cycles this is  
                        pwm_set_chan_level(audio_pin_slice2,audio_pin_channel2, round(findValue(buttonNum2,evenHarmonics2,oddHarmonics2,wav_position2>>1)/(evenHarmonics2+oddHarmonics2+1)));
                        wav_position2++;
                    } else {
                        
                        // reset to start
                        wav_position2 = 0;
                        
                        
                    }
                }
            }else if(frequency2<freqList[12]){
                if(vibrato2){
                    if (wav_position2 < (WAV_DATA_LENGTH<<4) - 1) { 
                        // set pwm level 
                        // allow the pwm value to repeat for 8 cycles this is  
                        pwm_set_chan_level(audio_pin_slice2,audio_pin_channel2, round(findValue(buttonNum2,evenHarmonics2,oddHarmonics2,wav_position2>>4)/(evenHarmonics2+oddHarmonics2+1)));
                        wav_position2++;
                    } else {
                        // reset to start
                        wav_position2 = 0;
                        if(vibUP2){
                            if(currentF2<upperVibrato2){
                                currentF2+=vibchangeParam2;
                            } else{
                                vibUP2=false;
                            }
                            updateClockDiv(clockDivChange(currentF2),AUDIO_PIN2,audio_pin_slice2);
                        } else{
                            if(currentF2>lowerVibrato2){
                                currentF2-=vibchangeParam2;
                            } else{
                                vibUP2=true;
                            }
                            updateClockDiv(clockDivChange(currentF2),AUDIO_PIN2,audio_pin_slice2);
                        }
                        
                    }
                }else{
                    if (wav_position2 < (WAV_DATA_LENGTH<<4) - 1) { 
                        // set pwm level 
                        // allow the pwm value to repeat for 8 cycles this is  
                        pwm_set_chan_level(audio_pin_slice2,audio_pin_channel2, round(findValue(buttonNum2,evenHarmonics2,oddHarmonics2,wav_position2>>4)/(evenHarmonics2+oddHarmonics2+1)));
                        wav_position2++;
                    } else {
                        
                        // reset to start
                        wav_position2 = 0;
                        
                        
                    }
                }
            }else if(frequency2<freqList[24]){
                if(vibrato2){
                    if (wav_position2 < (WAV_DATA_LENGTH<<3) - 1) { 
                        // set pwm level 
                        // allow the pwm value to repeat for 8 cycles this is  
                        pwm_set_chan_level(audio_pin_slice2,audio_pin_channel2, round(findValue(buttonNum2,evenHarmonics2,oddHarmonics2,wav_position2>>3)/(evenHarmonics2+oddHarmonics2+1)));
                        wav_position2++;
                    } else {
                        // reset to start
                        wav_position2 = 0;
                        if(vibUP2){
                            if(currentF2<upperVibrato2){
                                currentF2+=vibchangeParam2;
                            } else{
                                vibUP2=false;
                            }
                            updateClockDiv(clockDivChange(currentF2),AUDIO_PIN2,audio_pin_slice2);
                        } else{
                            if(currentF2>lowerVibrato2){
                                currentF2-=vibchangeParam2;
                            } else{
                                vibUP2=true;
                            }
                            updateClockDiv(clockDivChange(currentF2),AUDIO_PIN2,audio_pin_slice2);
                        }
                        
                    }
                }else{
                    if (wav_position2 < (WAV_DATA_LENGTH<<3) - 1) { 
                        // set pwm level 
                        // allow the pwm value to repeat for 8 cycles this is  
                        pwm_set_chan_level(audio_pin_slice2,audio_pin_channel2, round(findValue(buttonNum2,evenHarmonics2,oddHarmonics2,wav_position2>>3)/(evenHarmonics2+oddHarmonics2+1)));
                        wav_position2++;
                    } else {
                        
                        // reset to start
                        wav_position2 = 0;
                        
                        
                    }
                }
            }else{
                if(vibrato2){
                    if (wav_position2 < (WAV_DATA_LENGTH<<2) - 1) { 
                        // set pwm level 
                        // allow the pwm value to repeat for 8 cycles this is  
                        pwm_set_chan_level(audio_pin_slice2,audio_pin_channel2, round(findValue(buttonNum2,evenHarmonics2,oddHarmonics2,wav_position2>>2)/(evenHarmonics2+oddHarmonics2+1)));
                        wav_position2++;
                    } else {
                        // reset to start
                        wav_position2 = 0;
                        if(vibUP2){
                            if(currentF2<upperVibrato2){
                                currentF2+=vibchangeParam2;
                            } else{
                                vibUP2=false;
                            }
                            updateClockDiv(clockDivChange(currentF2),AUDIO_PIN2,audio_pin_slice2);
                        } else{
                            if(currentF2>lowerVibrato2){
                                currentF2-=vibchangeParam2;
                            } else{
                                vibUP2=true;
                            }
                            updateClockDiv(clockDivChange(currentF2),AUDIO_PIN2,audio_pin_slice2);
                        }
                        
                    }
                }else{
                    if (wav_position2 < (WAV_DATA_LENGTH<<2) - 1) { 
                        // set pwm level 
                        // allow the pwm value to repeat for 8 cycles this is  
                        pwm_set_chan_level(audio_pin_slice2,audio_pin_channel2, round(findValue(buttonNum2,evenHarmonics2,oddHarmonics2,wav_position2>>2)/(evenHarmonics2+oddHarmonics2+1)));
                        wav_position2++;
                    } else {
                        
                        // reset to start
                        wav_position2 = 0;
                        
                        
                    }
                }
            }
           
        }
    }else{
        //this is in the documentation...cancel raspberry pi? what?
        pwm_retard_count(1);
    }
    

    
}

int main(void) {
    /* Overclocking for fun but then also so the system clock is a 
     * multiple of typical audio sampling rates.
     */
    stdio_init_all();
    adc_init();
    adc_gpio_init(ADC_PIN);
    adc_gpio_init(ADC_PIN2);
    adc_select_input(1);


    gpio_init(GATE);
    gpio_set_dir(GATE,GPIO_IN);
    gpio_pull_down(GATE);
    gpio_set_irq_enabled(GATE,GPIO_IRQ_EDGE_RISE|GPIO_IRQ_EDGE_FALL,true);
    gpio_add_raw_irq_handler_masked(( 0x01 << GATE),&rawHandler1);

    gpio_init(GATE2);
    gpio_set_dir(GATE2,GPIO_IN);
    gpio_pull_down(GATE2);
    gpio_set_irq_enabled(GATE2,GPIO_IRQ_EDGE_RISE|GPIO_IRQ_EDGE_FALL,true);
    gpio_add_raw_irq_handler_masked(( 0x01 << GATE2),&rawHandler1);

    button_t *sine = create_button(SINE, onchange);
    button_t *square = create_button(SQUARE, onchange);
    button_t *triangle = create_button(TRIANGLE, onchange);
    button_t *porabola = create_button(PORABOLA, onchange);
    button_t *sawtooth= create_button(SAWTOOTH, onchange);
    button_t *hm_even_up = create_button(HM_EVEN_UP, onchange);
    button_t *hm_even_down = create_button(HM_EVEN_DOWN, onchange);
    button_t *hm_odd_up = create_button(HM_ODD_UP, onchange);
    button_t *hm_odd_down = create_button(HM_ODD_DOWN, onchange);
    button_t *vibrato = create_button(VIBRATO_PIN, onchange);
    button_t *switchsignal = create_button(SWITCHSIGNAL, onchange);
    irq_set_enabled(IO_IRQ_BANK0, true);
    vreg_set_voltage(VREG_VOLTAGE_1_05);
    set_sys_clock_khz(clockFreq, true); 
    gpio_set_function(AUDIO_PIN, GPIO_FUNC_PWM);
    gpio_set_function(AUDIO_PIN2, GPIO_FUNC_PWM);

    audio_pin_slice = pwm_gpio_to_slice_num(AUDIO_PIN);
    audio_pin_channel = pwm_gpio_to_channel(AUDIO_PIN);
    // Setup PWM interrupt to fire when PWM cycle is complete
    pwm_clear_irq(audio_pin_slice);
    pwm_set_irq_enabled(audio_pin_slice, true);
    // set the handle function above
    irq_set_exclusive_handler(PWM_IRQ_WRAP, pwm_interrupt_handler); 
    irq_set_enabled(PWM_IRQ_WRAP, true);
 
    // Setup PWM for audio output
    config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, clkDiv); 
    pwm_config_set_wrap(&config, wrap); 
    //pwm_config_set_phase_correct(&config,true);
    pwm_init(audio_pin_slice, &config, true);
    
    pwm_set_gpio_level(AUDIO_PIN, 0);



    audio_pin_slice2 = pwm_gpio_to_slice_num(AUDIO_PIN2);
    audio_pin_channel2 = pwm_gpio_to_channel(AUDIO_PIN2);
    // Setup PWM interrupt to fire when PWM cycle is complete
    pwm_clear_irq(audio_pin_slice2);
    pwm_set_irq_enabled(audio_pin_slice2, true);
    // set the handle function above
    irq_set_exclusive_handler(PWM_IRQ_WRAP, pwm_interrupt_handler); 
    irq_set_enabled(PWM_IRQ_WRAP, true);
 
    // Setup PWM for audio output
    config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, clkDiv); 
    pwm_config_set_wrap(&config, wrap); 
    //pwm_config_set_phase_correct(&config,true);
    pwm_init(audio_pin_slice2, &config, true);

    pwm_set_gpio_level(AUDIO_PIN2, 0);
    while(1) {
        __wfi(); // Wait for Interrupt
    }
}
void rawHandler1(){
    if(gpio_get_irq_event_mask(GATE) & GPIO_IRQ_EDGE_RISE){
        gpio_acknowledge_irq(GATE, GPIO_IRQ_EDGE_RISE );
        adc_select_input(1);
        adc_value=((adc_read())*conversionfactor);
        subScript=round(60*adc_value/3);
        if(subScript>60){
            frequency=freqList[0];
        }else if(subScript<0){
            frequency=freqList[60];
        }else{
            frequency=freqList[subScript];
        }
        currentF = freqList[subScript];
        if(subScript==60){
            upperVibrato=highestFrequency;
            lowerVibrato=freqList[subScript-1];
        }else if(subScript==0){
            upperVibrato=freqList[subScript+1];
            lowerVibrato=lowestFrequency;
        }else{
            upperVibrato=freqList[subScript+1];
            lowerVibrato=freqList[subScript-1];
        }
        vibchangeParam = (upperVibrato-lowerVibrato)/24;
        
        updateClockDiv(clockDivChange(frequency),AUDIO_PIN,audio_pin_slice);
        PLAY=true;
    }else if(gpio_get_irq_event_mask(GATE) & GPIO_IRQ_EDGE_FALL){
        gpio_acknowledge_irq(GATE, GPIO_IRQ_EDGE_FALL );
        PLAY=false;
    }
    else if(gpio_get_irq_event_mask(GATE2) & GPIO_IRQ_EDGE_RISE){
        adc_select_input(2);
        gpio_acknowledge_irq(GATE2, GPIO_IRQ_EDGE_RISE );
        adc_value=((adc_read())*conversionfactor);
        subScript2=round(60*adc_value/3);
        if(subScript2>60){
            frequency2=freqList[0];
        }else if(subScript2<0){
            frequency2=freqList[60];
        }else{
            frequency2=freqList[subScript2];
        }
        currentF2 = freqList[subScript2];
        if(subScript2==60){
            upperVibrato2=highestFrequency;
            lowerVibrato2=freqList[subScript2-1];
        }else if(subScript2==0){
            upperVibrato2=freqList[subScript2+1];
            lowerVibrato2=lowestFrequency;
        }else{
            upperVibrato2=freqList[subScript2+1];
            lowerVibrato2=freqList[subScript2-1];
        }
        vibchangeParam2 = (upperVibrato2-lowerVibrato2)/24;
        
        updateClockDiv(clockDivChange(frequency2),AUDIO_PIN2,audio_pin_slice2);
        PLAY2=true;
    }else if(gpio_get_irq_event_mask(GATE2) & GPIO_IRQ_EDGE_FALL){
        gpio_acknowledge_irq(GATE2, GPIO_IRQ_EDGE_FALL );
        PLAY2=false;
    }
}
