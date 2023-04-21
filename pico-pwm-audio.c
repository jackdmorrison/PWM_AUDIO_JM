#include <stdio.h>
#include <math.h>
#include "button.c"
#include <pico/stdlib.h>    
#include "hardware/irq.h"  
#include "hardware/pwm.h"  
#include "hardware/sync.h" 
#include "hardware/adc.h"
#include "hardware/vreg.h"

//Macros for Pin numbers
#define AUDIO_PIN  16
#define AUDIO_PIN2 18

#define ADC_PIN 28
#define ADC_PIN2 27

#define LED 25

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
#define INTONATION 14
#define VIBRATO_PIN 0

#define GATE 20
#define GATE2 21

//header file with wave frequency, configuration and level arrays
#include "waves.h"
//global variables for the PWM slices
uint audio_pin_slice;
uint audio_pin_slice2;
//global variables for Audio channels 
uint audio_pin_channel;
uint audio_pin_channel2;

//handler function declaration
void rawHandler1();

//global wave postion variables
int wav_position = 0;
int wav_position2=0;
//global adc variable
float adc_value=0;
//adc vref/adc range to give the conversion factor for adc_read to volts(float)
const float conversionfactor=3.3f/(1<<12);

bool vibrato = false; //vibrato on or off
bool vibrato2 = false; //vibrato on or off

//subscript variables for freqList array 
int subScript = 0;
int subScript2=0;

bool just=false;//just intonation on or off

//lower frequency in vibrato
float lowerVibrato=lowestFrequency;
float lowerVibrato2=lowestFrequency;
//global frequency variables for each Output
float frequency;
float frequency2;

//currentF used to determine frequency in vibrato
float currentF ;
float currentF2 ;

// upper frequency in vibrato
float upperVibrato;
float upperVibrato2;

//rate of change in frequency per wave cycle of vibrato Variable
float vibchangeParam ;
float vibchangeParam2;

//buttonNum used to determine level input array 
int buttonNum = 0;
int buttonNum2 = 0;

//number of even harmonics to be added to wave
int evenHarmonics=0;
int evenHarmonics2=0;
//number of off harmonics to be added to wave
int oddHarmonics=0;
int oddHarmonics2=0;
//variable to determine which pwm slice corresponds to an interupt
int irq;

//used to determine bitshift for each octave to increase number of repetitions of level array values
int val=1;
int val2=1;

bool vibUP=true;//vibrato move up or down in frequency
bool vibUP2=true;//vibrato move up or down in frequency

bool signal1=true;//buttons effects for signal 1(true) or 2(false)


float clockDivChange( float newFrequency){
    if(newFrequency>freqListJust[48]){
        return (WAV_FREQUENCY/newFrequency)*clkDiv*4;
    }
    else if(newFrequency>freqListJust[32]){
        return (WAV_FREQUENCY/newFrequency)*clkDiv*2;
    }else if(newFrequency<freqListJust[12]){
        return (WAV_FREQUENCY/newFrequency)*clkDiv/4;
    }else if(newFrequency<freqListJust[24]){
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



float findValue(int buttonNumber,int evenHarmonicsNum,int oddHarmonicsNum,int wave_position){
    int value=0;
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
        }else{
            if(oddHarmonics2!=0){
                oddHarmonics2--;
            }
        }
        break;
    case VIBRATO_PIN:
        if(signal1){
            if(vibrato){
                if(frequency>freqListJust[48]){
                    val=0;
                }else if(frequency>freqListJust[32]){
                    val=1;
                }else if(frequency<freqListJust[12]){
                    val=4;
                }else if(frequency<freqListJust[24]){
                    val=3;
                }else{
                    val=2;
                }
                vibrato=false;
            }else{
                vibrato=true;
            }
        }else{
            
            if(vibrato2){
                if(frequency2>freqListJust[48]){
                    val2=0;
                }
                else if(frequency2>freqListJust[32]){
                    val2=1;
                }else if(frequency2<freqListJust[12]){
                    val2=4;
                }else if(frequency2<freqListJust[24]){
                    val2=3;
                }else{
                    val2=2;
                }
                vibrato2=false;
            }else{
                vibrato2=true;
            }
        }
        
        break;
    case SWITCHSIGNAL:
        if(signal1){
            gpio_put(LED, false);
            signal1=false;
        }else{
            gpio_put(LED, true);
            signal1=true;
        }
        break;
    case INTONATION:
        if(just){
            just=false;
        }else{
            just=true;
        }
        break;
  }
}


void pwm_interrupt_handler() {
    
    irq= pwm_get_irq_status_mask();
    if(irq & (1<<0)){
    //if(gpio_get_irq_event_mask(AUDIO_PIN)){
        pwm_clear_irq(0);
        
        if (wav_position < (WAV_DATA_LENGTH<<val) - 1) { 
            // set pwm level 
            // allow the pwm value to repeat for 2 cycles this is  
            
            pwm_set_chan_level(audio_pin_slice,audio_pin_channel, round(findValue(buttonNum,evenHarmonics,oddHarmonics,wav_position>>val)/(evenHarmonics+oddHarmonics+1)));
            wav_position++;
        } else {
            // reset to start
            wav_position = 0;
            if(vibrato){
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
                    if(currentF>freqListJust[48]){
                        val=0;
                    }else if(currentF>freqListJust[32]){
                        val=1;
                    }else if(currentF<freqListJust[12]){
                        val=4;
                    }else if(currentF<freqListJust[24]){
                        val=3;
                    }else{
                        val=2;
                    }
                }
                
            }
        }  
    }else if(irq & (1<<1)){
        pwm_clear_irq(1);
    
        if (wav_position2 < (WAV_DATA_LENGTH<<val) - 1) { 
            // set pwm level 
            // allow the pwm value to repeat for 2 cycles  
            pwm_set_chan_level(audio_pin_slice2,audio_pin_channel2, round(findValue(buttonNum2,evenHarmonics2,oddHarmonics2,wav_position2>>val)/(evenHarmonics2+oddHarmonics2+1)));
            wav_position2++;
        } else {
            // reset to start
            wav_position2 = 0;
            if(vibrato2){
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
                if(currentF2>freqListJust[48]){
                    val2=0;
                }
                else if(currentF2>freqListJust[32]){
                    val2=1;
                }else if(currentF2<freqListJust[12]){
                    val2=4;
                }else if(currentF2<freqListJust[24]){
                    val2=3;
                }else{
                    val2=2;
                }
            }
        }
    }
    

    
}

int main(void) {
    //
    float frequency=freqListJust[0];
    float frequency2=freqListJust[0];

    float currentF = freqListJust[0];
    float currentF2 = freqListJust[0];

    float upperVibrato=freqListJust[0];
    float upperVibrato2=freqListJust[0];
    float vibchangeParam = (freqListJust[0]-lowestFrequency)/24;
    float vibchangeParam2 = (freqListJust[0]-lowestFrequency)/24;

    /* Overclocking for fun but then also so the system clock is a 
     * multiple of typical audio sampling rates.
     */
    gpio_init(LED);
    gpio_set_dir(LED, GPIO_OUT);
    gpio_put(LED, true);
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
    button_t *vibratoB = create_button(VIBRATO_PIN, onchange);
    button_t *switchsignal = create_button(SWITCHSIGNAL, onchange);
    button_t *intonation = create_button(INTONATION, onchange);
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
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, clkDiv); 
    pwm_config_set_wrap(&config, wrap); 
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
    pwm_config config2 = pwm_get_default_config();
    pwm_config_set_clkdiv(&config2, clkDiv); 
    pwm_config_set_wrap(&config2, wrap); 
    pwm_init(audio_pin_slice2, &config2, true);

    pwm_set_gpio_level(AUDIO_PIN2, 0);
    while(1) {
        __wfi(); // Wait for Interrupt
    }
}
void rawHandler1(){
    if(gpio_get_irq_event_mask(GATE) & GPIO_IRQ_EDGE_RISE){
        gpio_acknowledge_irq(GATE, GPIO_IRQ_EDGE_RISE );
        adc_select_input(2);
        adc_value=((adc_read())*conversionfactor);
        subScript=round(60*adc_value/3);
        if(just){
            if(subScript>=60){
                frequency=freqListJust[60];
            }else if(subScript<=0){
                frequency=freqListJust[0];
            }else{
                frequency=freqListJust[subScript];
            }
            currentF = freqListJust[subScript];
            if(subScript==60){
                upperVibrato=highestFrequency;
                lowerVibrato=freqListJust[subScript-1];
            }else if(subScript==0){
                upperVibrato=freqListJust[subScript+1];
                lowerVibrato=lowestFrequency;
            }else{
                upperVibrato=freqListJust[subScript+1];
                lowerVibrato=freqListJust[subScript-1];
            }
        }else{
            if(subScript>=60){
                frequency=freqListEqualT[60];
            }else if(subScript<=0){
                frequency=freqListEqualT[0];
            }else{
                frequency=freqListEqualT[subScript];
            }
            currentF = freqListEqualT[subScript];
            if(subScript==60){
                upperVibrato=highestFrequency;
                lowerVibrato=freqListEqualT[subScript-1];
            }else if(subScript==0){
                upperVibrato=freqListEqualT[subScript+1];
                lowerVibrato=lowestFrequency;
            }else{
                upperVibrato=freqListEqualT[subScript+1];
                lowerVibrato=freqListEqualT[subScript-1];
            }
        }
        if(frequency>freqListJust[48]){
            val=0;
        }else if(frequency>freqListJust[32]){
            val=1;
        }else if(frequency<freqListJust[12]){
            val=4;
        }else if(frequency<freqListJust[24]){
            val=3;
        }else{
            val=2;
        }
        vibchangeParam = 3*(upperVibrato-lowerVibrato)/currentF;
        
        
        
        updateClockDiv(clockDivChange(frequency),AUDIO_PIN,audio_pin_slice);
        
    }else if(gpio_get_irq_event_mask(GATE) & GPIO_IRQ_EDGE_FALL){
        gpio_acknowledge_irq(GATE, GPIO_IRQ_EDGE_FALL );
        
    }
    if(gpio_get_irq_event_mask(GATE2) & GPIO_IRQ_EDGE_RISE){
        adc_select_input(1);
        gpio_acknowledge_irq(GATE2, GPIO_IRQ_EDGE_RISE );
        adc_value=((adc_read())*conversionfactor);
        subScript2=round(60*adc_value/3);
        //check if just or equal tempered
        if(just){
            //set values frequency for just tuning
            if(subScript2>=60){
                frequency2=freqListJust[60];
            }else if(subScript2<=0){
                frequency2=freqListJust[0];
            }else{
                frequency2=freqListJust[subScript2];
            }
            //finds the current frequency 
            currentF2 = freqListJust[subScript2];
            //set upper and lower vibrato frequency variables
            if(subScript2==60){
                //upper set to semitone above freqListJust[60]
                upperVibrato2=highestFrequency;
                lowerVibrato2=freqListJust[subScript2-1];
            }else if(subScript2==0){
                upperVibrato2=freqListJust[subScript2+1];
                //upper set to semitone below freqListJust[0]
                lowerVibrato2=lowestFrequency;
            }else{
                //upper set to semitone above
                upperVibrato2=freqListJust[subScript2+1];
                //lower set for semitone below
                lowerVibrato2=freqListJust[subScript2-1];
            }
        }else{
            //set values frequency for equal tempered tuning
            if(subScript2>=60){
                frequency2=freqListEqualT[60];
            }else if(subScript2<=0){
                frequency2=freqListEqualT[0];
            }else{
                frequency2=freqListEqualT[subScript2];
            }
             //finds the current frequency 
            currentF2 = freqListEqualT[subScript2];
            if(subScript2==60){
                upperVibrato2=highestFrequency;
                lowerVibrato2=freqListEqualT[subScript2-1];
            }else if(subScript2==0){
                upperVibrato2=freqListEqualT[subScript2+1];
                lowerVibrato2=lowestFrequency;
            }else{
                upperVibrato2=freqListEqualT[subScript2+1];
                lowerVibrato2=freqListEqualT[subScript2-1];
            }
        }
        vibchangeParam2 = 3*(upperVibrato2-lowerVibrato2)/currentF2;
        if(frequency2>freqListJust[48]){
            val2=0;
        }
        else if(frequency2>freqListJust[32]){
            val2=1;
        }else if(frequency2<freqListJust[12]){
            val2=4;
        }else if(frequency2<freqListJust[24]){
            val2=3;
        }else{
            val2=2;
        }
        
        updateClockDiv(clockDivChange(frequency2),AUDIO_PIN2,audio_pin_slice2);
        
    }else if(gpio_get_irq_event_mask(GATE2) & GPIO_IRQ_EDGE_FALL){
        gpio_acknowledge_irq(GATE2, GPIO_IRQ_EDGE_FALL );
    }
}
