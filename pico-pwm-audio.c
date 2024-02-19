#include <stdio.h>
#include <math.h>

#include <pico/stdlib.h>    
#include "hardware/irq.h"  //interupts
#include "hardware/pwm.h"  //pwm
#include "hardware/sync.h" 
//#include "hardware/adc.h"//adc
#include "hardware/vreg.h"
#include "button.c"
//header file with wave frequency, configuration and level arrays
#include "configuration.h"
//Macros for Pin numbers
#define AUDIO_PIN  16
#define Note0 0
#define Note1 1
#define Note2 2
#define Note3 3
#define Note4 4
#define Note5 5
#define Note6 6
#define Note7 7
#define Note8 8
#define Note9 9
#define Note10 10
#define Note11 11
#define Octave0 28
#define Octave1 27
#define Octave2 26
#define Octave3 22
#define Octave4 21
#define LED 25


//global variables for the PWM slices
uint audio_pin_slice;
//uint audio_pin_slice2;
//global variables for Audio channels 
uint audio_pin_channel;
//uint audio_pin_channel2;

//handler function declaration
void rawHandler1();

//global wave postion variables
int wav_position = 0;
//int wav_position2=0;

//adc vref/adc range to give the conversion factor for adc_read to volts(float)
const float conversionfactor=3.3f/(1<<12);

//bool vibrato = false; //vibrato on or off
//bool vibrato2 = false; //vibrato on or off

//subscript variables for freqList array 
//int subScript = 0;
//int subScript2=0;

//bool just=false;//just intonation on or off

//lower frequency in vibrato
//float lowerVibrato=lowestFrequency;
//float lowerVibrato2=lowestFrequency;
//global frequency variables for each Output
float frequency;
//float frequency2;

//currentF used to determine frequency in vibrato
float currentF ;
//float currentF2 ;

// upper frequency in vibrato
//float upperVibrato;
//float upperVibrato2;

//rate of change in frequency per wave cycle of vibrato Variable
//float vibchangeParam ;
//float vibchangeParam2;

//buttonNum used to determine level input array 
int buttonNum = 0;
//int buttonNum2 = 0;

//number of even harmonics to be added to wave
//int evenHarmonics=0;
//int evenHarmonics2=0;
//number of off harmonics to be added to wave
//int oddHarmonics=0;
//int oddHarmonics2=0;
//variable to determine which pwm slice corresponds to an interupt
int irq;

//used to determine bitshift for each octave to increase number of repetitions of level array values
int val=1;
int val2=1;

//bool vibUP=true;//vibrato move up or down in frequency
//bool vibUP2=true;//vibrato move up or down in frequency

//bool effects1=true;//buttons effects for effects 1(true) or 2(false)
float level;

//function to update hardware with new clock divider based on the newFrequency
//also update the val variable for each octave
void updateClockDiv(int PIN,int pin_slice, float newFrequency){
    float clockDiv=0;
    //Octave 5
    if(newFrequency>freqListJust[48]){
        //find clock new clock divider
        clockDiv=(WAV_FREQUENCY/newFrequency)*clkDiv*4;
        //find the bitshift value 
        if(pin_slice==audio_pin_slice){
            val=0;
        }else{
            val2=0;
        }
    }
    //Octave 4
    else if(newFrequency>freqListJust[32]){
        //find clock new clock divider
        clockDiv=(WAV_FREQUENCY/newFrequency)*clkDiv*2;
        //find the bitshift value 
        if(pin_slice==audio_pin_slice){
            val=1;
        }else{
            val2=1;
        }
    }
    //Octave 1
    else if(newFrequency<freqListJust[12]){
        //find clock new clock divider
        clockDiv=(WAV_FREQUENCY/newFrequency)*clkDiv/4;
        //find the bitshift value 
        if(pin_slice==audio_pin_slice){
            val=4;
        }else{
            val2=4;
        }
    }
    //Octave 2
    else if(newFrequency<freqListJust[24]){
        //find clock new clock divider
        clockDiv=(WAV_FREQUENCY/newFrequency)*clkDiv/2;
        //find the bitshift value 
        if(pin_slice==audio_pin_slice){
            val=3;
        }else{
            val2=3;
        }
    }
    //Octave 3
    else{
        //find clock new clock divider
        clockDiv=(WAV_FREQUENCY/newFrequency)*clkDiv;
        //find the bitshift value 
        if(pin_slice==audio_pin_slice){
            val=2;
        }else{
            val2=2;
        }
    }
    //check if within limits and update hardware configuration
    if(1.f<=clkDiv<=256.f){
        pwm_set_clkdiv(pin_slice, clkDiv); 
        pwm_set_gpio_level(PIN, 0);
    }//ensure limits are represented
    else if(clkDiv<1.f){
        pwm_set_clkdiv(pin_slice, 1.f); 
        pwm_set_gpio_level(PIN, 0);
    }
    else if(clkDiv>256.f){
        pwm_set_clkdiv(pin_slice, 256.f); 
        pwm_set_gpio_level(PIN, 0);
    }
    
}


//find current level of pin based on which effect, harmonics and wavepostion
float findValue(int buttonNumber,int wave_position){
    //value variable used to determine current level
    int value=0;
    value=SIN_WAV_DATA[wave_position];//1
    value+=HARMONIC2_WAV_DATA[wave_position];
    value+=HARMONIC3_WAV_DATA[wave_position];
    value+=HARMONIC4_WAV_DATA[wave_position];
    value+=HARMONIC5_WAV_DATA[wave_position];
    value+=HARMONIC6_WAV_DATA[wave_position];
    value+=HARMONIC7_WAV_DATA[wave_position];
    value+=HARMONIC8_WAV_DATA[wave_position];

    //switch case to determine effect 
    // switch (buttonNumber){
    //     case 0: //sin wave
    //         value=SIN_WAV_DATA[wave_position];
    //         break;
    //     case 1: //square wave
    //         value=SQR_WAV_DATA[wave_position];
    //         break;
    //     case 2: //Triangle wave
    //         value=TRI_WAV_DATA[wave_position];
    //         break;
    //     case 3: //Saw wave
    //         value=SAW_WAV_DATA[wave_position];
    //         break;
    //     case 4: //Reverse Saw wave
    //         value=R_SAW_WAV_DATA[wave_position];
    //         break;
    //     case 5://porabola wave
    //         value=PRBA_WAV_DATA[wave_position];
    //         break;
    // }
    //switch case for number of even harmonics to add
    // switch(evenHarmonicsNum){
    //     case 0://add 0 harmonics
    //         break;
    //     case 1://add first harmonic
    //         value=value+HARMONIC2_WAV_DATA[wave_position];
    //         break;
    //     case 2://add 2 + 4 harmonics
    //         value=value+HARMONIC2_WAV_DATA[wave_position]+HARMONIC4_WAV_DATA[wave_position];
    //         break;
    //     case 3://add 2 + 4 + 6 harmonics
    //         value=value+HARMONIC2_WAV_DATA[wave_position]+HARMONIC4_WAV_DATA[wave_position]+HARMONIC6_WAV_DATA[wave_position];
    //         break;
    //     case 4://add 2 + 4 + 6 + 8 harmonics
    //         value=value+HARMONIC2_WAV_DATA[wave_position]+HARMONIC4_WAV_DATA[wave_position]+HARMONIC6_WAV_DATA[wave_position]+HARMONIC8_WAV_DATA[wave_position];
    //         break;
    //     case 5://add 2 + 4 + 6 + 8 + 10 harmonics
    //         value=value+HARMONIC2_WAV_DATA[wave_position]+HARMONIC4_WAV_DATA[wave_position]+HARMONIC6_WAV_DATA[wave_position]+HARMONIC8_WAV_DATA[wave_position]+HARMONIC10_WAV_DATA[wave_position];
    //         break;
    //     case 6://add 2 + 4 + 6 + 8 + 10 + 12 harmonics
    //         value=value+HARMONIC2_WAV_DATA[wave_position]+HARMONIC4_WAV_DATA[wave_position]+HARMONIC6_WAV_DATA[wave_position]+HARMONIC8_WAV_DATA[wave_position]+HARMONIC10_WAV_DATA[wave_position]+HARMONIC12_WAV_DATA[wave_position];
    //         break;
    // }
    // //switch case to add odd harmonics
    // switch(oddHarmonicsNum){
    //     case 0://add 0 harmonics
    //         break;
    //     case 1://add 3 harmonics
    //         value=value+HARMONIC3_WAV_DATA[wave_position];
    //         break;
    //     case 2://add 3 + 5 harmonics
    //         value=value+HARMONIC3_WAV_DATA[wave_position]+HARMONIC5_WAV_DATA[wave_position];
    //         break;
    //     case 3://add 3 + 5 + 7  harmonics
    //         value=value+HARMONIC3_WAV_DATA[wave_position]+HARMONIC5_WAV_DATA[wave_position]+HARMONIC7_WAV_DATA[wave_position];
    //         break;
    //     case 4://add 3 + 5 + 7 + 9 harmonics
    //         value=value+HARMONIC3_WAV_DATA[wave_position]+HARMONIC5_WAV_DATA[wave_position]+HARMONIC7_WAV_DATA[wave_position]+HARMONIC9_WAV_DATA[wave_position];
    //         break;
    //     case 5://add 3 + 5 + 7 + 9 + 11 harmonics
    //         value=value+HARMONIC3_WAV_DATA[wave_position]+HARMONIC5_WAV_DATA[wave_position]+HARMONIC7_WAV_DATA[wave_position]+HARMONIC9_WAV_DATA[wave_position]+HARMONIC11_WAV_DATA[wave_position];
    //         break;
    // }
    //return level needed to achieve effect parameters
    return value;
}
//handler function for buttons
void onchange(button_t *button_p) {
    button_t *button = (button_t*)button_p;

    if(button->state) return; // if released retun
    
    //swith case to determine the button pin
    switch(button->pin){
        case Note0://update buttonNum for Sine wave
            frequency=freqListEqualT[0+(buttonNum*12)];
            updateClockDiv(AUDIO_PIN,audio_pin_slice,frequency);
            break;
        case Note1://update buttonNum for Square wave
        
            frequency=freqListEqualT[1+(buttonNum*12)];
            updateClockDiv(AUDIO_PIN,audio_pin_slice,frequency);
            break;
        case Note2://update buttonNum for Triangle wave
            frequency=freqListEqualT[2+(buttonNum*12)];
            updateClockDiv(AUDIO_PIN,audio_pin_slice,frequency);
            break;
        case Note3://update buttonNum for Sawtooth wave
            frequency=freqListEqualT[3+(buttonNum*12)];
            updateClockDiv(AUDIO_PIN,audio_pin_slice,frequency);
            break;
        case Note4://update buttonNum for porabola wave
            frequency=freqListEqualT[4+(buttonNum*12)];
            updateClockDiv(AUDIO_PIN,audio_pin_slice,frequency);
            break;
        case Note5://increase even harmonic variable
            frequency=freqListEqualT[5+(buttonNum*12)];
            updateClockDiv(AUDIO_PIN,audio_pin_slice,frequency);
            break;
        case Note6://decrease odd harmonics variable
            frequency=freqListEqualT[6+(buttonNum*12)];
            updateClockDiv(AUDIO_PIN,audio_pin_slice,frequency);
            break;
        case Note7://increase odd harmonic variable
            frequency=freqListEqualT[7+(buttonNum*12)];
            updateClockDiv(AUDIO_PIN,audio_pin_slice,frequency);
            break;
        case Note8://decrease odd harmonic variable
            frequency=freqListEqualT[8+(buttonNum*12)];
            updateClockDiv(AUDIO_PIN,audio_pin_slice,frequency);
            break;
        case Note9://toggle vibrato on and off 
            frequency=freqListEqualT[9+(buttonNum*12)];
            updateClockDiv(AUDIO_PIN,audio_pin_slice,frequency);
            break;
        case Note10:
            frequency=freqListEqualT[10+(buttonNum*12)];
            updateClockDiv(AUDIO_PIN,audio_pin_slice,frequency);
            break;
        case Note11://toggle intonation between just and Equal tempered
            frequency=freqListEqualT[11+(buttonNum*12)];
            updateClockDiv(AUDIO_PIN,audio_pin_slice,frequency);
            break;
        case Octave0://change octave
            buttonNum=0;
            break;
        case Octave1://change octave
            buttonNum=1;
            break;
        case Octave2://change octave
            buttonNum=2;
            break;
        case Octave3://change octave
            buttonNum=3;
            break;
        case Octave4://change octave
            buttonNum=4;
            break;
    }
}

//handler function for PWM
void pwm_interrupt_handler() {
    //find interupt mask to determin slice
    irq= pwm_get_irq_status_mask();
    //check pin slice 
    //if(irq & (1<<0)){
        pwm_clear_irq(audio_pin_slice);
        /*
            if the wave postion is less than the wave position
            variable bit shift that modulates the number of repeated 
            values at each octave
        */
        if (wav_position < (WAV_DATA_LENGTH<<val) - 1) { 
            // set pwm level on slice and pin
            // allow the pwm value to repeat for 2^val cycles  
            level=findValue(buttonNum,wav_position>>val)/8;
            pwm_set_chan_level(audio_pin_slice,audio_pin_channel, round(level) );
            wav_position++;
        } else {
            // reset to start
            wav_position = 0;

            // if(vibrato){
            //     if(vibUP){
            //         //increase frequency until upper limit achieved
            //         if(currentF<upperVibrato){
            //             currentF+=vibchangeParam;
            //         } else{
            //             vibUP=false;
            //         }
            //     } else{
            //         //decrease frequency until lower limit achieved
            //         if(currentF>lowerVibrato){
            //             currentF-=vibchangeParam;
            //         } else{
            //             vibUP=true;
            //         } 
            //     }
            //     //update clock div with new frequency
            //     updateClockDiv(AUDIO_PIN,audio_pin_slice,currentF);
            // }
        }
    
    // }else if(irq & (1<<1)){
    //     pwm_clear_irq(1);
    //     /*
    //         if the wave postion is less than the wave position
    //         variable bit shift that modulates the number of repeated 
    //         values at each octave
    //     */
    //     if (wav_position2 < (WAV_DATA_LENGTH<<val) - 1) { 
    //         // set pwm level on slice and pin
    //         // allow the pwm value to repeat for 2^val cycles   
    //         level=findValue(buttonNum2,evenHarmonics2,oddHarmonics2,wav_position2>>val)/(evenHarmonics2+oddHarmonics2+1);
    //         pwm_set_chan_level(audio_pin_slice2,audio_pin_channel2, round(level));
    //         wav_position2++;
    //     } else {
    //         // reset to start
    //         wav_position2 = 0;
    //         if(vibrato2){
    //             //increase frequency until upper limit achieved
    //             if(vibUP2){
    //                 if(currentF2<upperVibrato2){
    //                     currentF2+=vibchangeParam2;
    //                 } else{
    //                     vibUP2=false;
    //                 }
    //             } else{
    //                 //decrease frequency until lower limit achieved
    //                 if(currentF2>lowerVibrato2){
    //                     currentF2-=vibchangeParam2;
    //                 } else{
    //                     vibUP2=true;
    //                 } 
    //             }
    //             //update clock div with new frequency
    //             updateClockDiv(AUDIO_PIN2,audio_pin_slice2,currentF2);
    //         }
    //     }
    //}
}

int main(void) {
    //set initial frequency
    frequency=freqListEqualT[0];
    //frequency2=freqListJust[0];

    //set current Frequency
    currentF = freqListEqualT[0];
    //currentF2 = freqListJust[0];
    //set upper vibrato
    //upperVibrato=freqListEqualT[1];
    //upperVibrato2=freqListEqualT[1];

    //set the initial increment value for vibrato 
    //vibchangeParam = 3*(upperVibrato-lowerVibrato)/currentF;
    //vibchangeParam2 = 3*(upperVibrato-lowerVibrato)/currentF;

    //initialise LED Pin
    gpio_init(LED);
    gpio_set_dir(LED, GPIO_OUT);
    gpio_put(LED, true);

    stdio_init_all();

    //initialise the ADC pins
    //adc_init();
    //adc_gpio_init(ADC_PIN);
    //adc_gpio_init(ADC_PIN2);
    //adc_select_input(1);

    //initilise gate pin
    //gpio_init(GATE);
    //gpio_set_dir(GATE,GPIO_IN);
    //gpio_pull_down(GATE);
    //enable interupts 
    //gpio_set_irq_enabled(GATE,GPIO_IRQ_EDGE_RISE|GPIO_IRQ_EDGE_FALL,true);
    //add handler for masked GATE pin
    //gpio_add_raw_irq_handler_masked(( 0x01 << GATE),&rawHandler1);

    //initilise gate2 pin
    //gpio_init(GATE2);
    //(GATE2,GPIO_IN);
    //(GATE2);
    //enable interupts
    //gpio_set_irq_enabled(GATE2,GPIO_IRQ_EDGE_RISE|GPIO_IRQ_EDGE_FALL,true);
    //add handler for masked GATE2 pin
    //gpio_add_raw_irq_handler_masked(( 0x01 << GATE2),&rawHandler1);

    //initialise button pins using jake rosomans button_t struct
    button_t *note0 = create_button(Note0, onchange);
    button_t *note1 = create_button(Note1, onchange);
    button_t *note2 = create_button(Note2, onchange);
    button_t *note3 = create_button(Note3, onchange);
    button_t *note4 = create_button(Note4, onchange);
    button_t *note5 = create_button(Note5, onchange);
    button_t *note6 = create_button(Note6, onchange);
    button_t *note7 = create_button(Note7, onchange);
    button_t *note8 = create_button(Note8, onchange);
    button_t *note9 = create_button(Note9, onchange);
    button_t *note10 = create_button(Note10, onchange);
    button_t *note11 = create_button(Note11, onchange);
    
    button_t *octave0 = create_button(Octave0, onchange);
    button_t *octave1 = create_button(Octave1, onchange);
    button_t *octave2 = create_button(Octave2, onchange);
    button_t *octave3 = create_button(Octave3, onchange);
    button_t *octave4 = create_button(Octave4, onchange);
    //enable the IO_IRQ_BANK0 for the GATEs interupts
    irq_set_enabled(IO_IRQ_BANK0, true);

    //increase voltage for stable overclocking
    vreg_set_voltage(VREG_VOLTAGE_1_05);

    //set clock frequency based on value from wave.h
    set_sys_clock_khz(clockFreq, true); 

    //set PWM pin function
    gpio_set_function(AUDIO_PIN, GPIO_FUNC_PWM);
    //gpio_set_function(AUDIO_PIN2, GPIO_FUNC_PWM);

    //find PWM pin slice and channel
    audio_pin_slice = pwm_gpio_to_slice_num(AUDIO_PIN);
    audio_pin_channel = pwm_gpio_to_channel(AUDIO_PIN);
    // Setup PWM interrupt to fire when PWM cycle is complete
    pwm_clear_irq(audio_pin_slice);
    pwm_set_irq_enabled(audio_pin_slice, true);
    // set the PWM handle function
    irq_set_exclusive_handler(PWM_IRQ_WRAP, pwm_interrupt_handler); 
 
    // Set PWM config with wrap and clock divider
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, clkDiv); 
    pwm_config_set_wrap(&config, wrap); 
    //initialise slice with config
    pwm_init(audio_pin_slice, &config, true);

    // Setup PWM interrupt to fire when PWM cycle is complete
    
    pwm_set_gpio_level(AUDIO_PIN, 0);


    //set up for secondary PWM output
    //audio_pin_slice2 = pwm_gpio_to_slice_num(AUDIO_PIN2);
    //audio_pin_channel2 = pwm_gpio_to_channel(AUDIO_PIN2);
    
    //pwm_clear_irq(audio_pin_slice2);
    //pwm_set_irq_enabled(audio_pin_slice2, true);
 
    // Set PWM config with wrap and clock divider
    //pwm_config config2 = pwm_get_default_config();
    //pwm_config_set_clkdiv(&config2, clkDiv); 
    //pwm_config_set_wrap(&config2, wrap); 
    //initialise slice with config
    //pwm_init(audio_pin_slice2, &config2, true);
    //irq_set_enabled(PWM_IRQ_WRAP, true);
    //pwm_set_gpio_level(AUDIO_PIN2, 0);

    while(1) {
        __wfi(); // Wait for Interrupt
    }
}
//handler function for GATE input
// void rawHandler1(){
//     //check for GATE one rise event
//     if(gpio_get_irq_event_mask(GATE) & GPIO_IRQ_EDGE_RISE){
//         //acknowledge the interrupt
//         gpio_acknowledge_irq(GATE, GPIO_IRQ_EDGE_RISE );
//         //select ADC 2 for read
//         adc_select_input(2);
//         //find voltage by multiplying the adc_read() by conversion factor
//         adc_value=((adc_read())*conversionfactor);
//         //find array subscript based on voltage
//         subScript=round(60*adc_value/3);
//         //set frequency for just intonation.
//         if(just){
//             //check extremes and represent them
//             if(subScript>=60){
//                 frequency=freqListJust[60];
//             }else if(subScript<=0){
//                 frequency=freqListJust[0];
//             }else{
//                 frequency=freqListJust[subScript];
//             }
//             //set currentF for vibrato
//             currentF = freqListJust[subScript];
//             //find upper and lower vibrato frequencies
//             if(subScript==60){
//                 upperVibrato=highestFrequency;
//                 lowerVibrato=freqListJust[subScript-1];
//             }else if(subScript==0){
//                 upperVibrato=freqListJust[subScript+1];
//                 lowerVibrato=lowestFrequency;
//             }else{
//                 upperVibrato=freqListJust[subScript+1];
//                 lowerVibrato=freqListJust[subScript-1];
//             }
//         }
//         //set frequency for Equal tempered intonation
//         else{
//             if(subScript>=60){
//                 frequency=freqListEqualT[60];
//             }else if(subScript<=0){
//                 frequency=freqListEqualT[0];
//             }else{
//                 frequency=freqListEqualT[subScript];
//             }
//             currentF = freqListEqualT[subScript];
//             if(subScript==60){
//                 upperVibrato=highestFrequency;
//                 lowerVibrato=freqListEqualT[subScript-1];
//             }else if(subScript==0){
//                 upperVibrato=freqListEqualT[subScript+1];
//                 lowerVibrato=lowestFrequency;
//             }else{
//                 upperVibrato=freqListEqualT[subScript+1];
//                 lowerVibrato=freqListEqualT[subScript-1];
//             }
//         }
//         //calculate the new increment value for vibrato 
//         vibchangeParam = 3*(upperVibrato-lowerVibrato)/currentF;
//         updateClockDiv(AUDIO_PIN,audio_pin_slice,frequency);
        
//     }else if(gpio_get_irq_event_mask(GATE) & GPIO_IRQ_EDGE_FALL){
//         gpio_acknowledge_irq(GATE, GPIO_IRQ_EDGE_FALL );
        
//     }
//     //check for GATE two rise event
//     if(gpio_get_irq_event_mask(GATE2) & GPIO_IRQ_EDGE_RISE){
//         //select ADC 1 for read
//         adc_select_input(1);
//         //acknowledge Interupt event
//         gpio_acknowledge_irq(GATE2, GPIO_IRQ_EDGE_RISE );
        
//         adc_value=((adc_read())*conversionfactor);
//         subScript2=round(60*adc_value/3);
//         //check if just or equal tempered
//         if(just){
//             //set values frequency for just tuning
//             if(subScript2>=60){
//                 frequency2=freqListJust[60];
//             }else if(subScript2<=0){
//                 frequency2=freqListJust[0];
//             }else{
//                 frequency2=freqListJust[subScript2];
//             }
//             //finds the current frequency 
//             currentF2 = freqListJust[subScript2];
//             //set upper and lower vibrato frequency variables
//             if(subScript2==60){
//                 //upper set to semitone above freqListJust[60]
//                 upperVibrato2=highestFrequency;
//                 lowerVibrato2=freqListJust[subScript2-1];
//             }else if(subScript2==0){
//                 upperVibrato2=freqListJust[subScript2+1];
//                 //upper set to semitone below freqListJust[0]
//                 lowerVibrato2=lowestFrequency;
//             }else{
//                 //upper set to semitone above
//                 upperVibrato2=freqListJust[subScript2+1];
//                 //lower set for semitone below
//                 lowerVibrato2=freqListJust[subScript2-1];
//             }
//         }else{
//             //set values frequency for equal tempered tuning
//             if(subScript2>=60){
//                 frequency2=freqListEqualT[60];
//             }else if(subScript2<=0){
//                 frequency2=freqListEqualT[0];
//             }else{
//                 frequency2=freqListEqualT[subScript2];
//             }
//              //finds the current frequency 
//             currentF2 = freqListEqualT[subScript2];
//             if(subScript2==60){
//                 upperVibrato2=highestFrequency;
//                 lowerVibrato2=freqListEqualT[subScript2-1];
//             }else if(subScript2==0){
//                 upperVibrato2=freqListEqualT[subScript2+1];
//                 lowerVibrato2=lowestFrequency;
//             }else{
//                 upperVibrato2=freqListEqualT[subScript2+1];
//                 lowerVibrato2=freqListEqualT[subScript2-1];
//             }
//         }
//         //calculate the new increment value for vibrato
//         vibchangeParam2 = 3*(upperVibrato2-lowerVibrato2)/currentF2;
//         //update the clock divider for new frequency
//         updateClockDiv(AUDIO_PIN2,audio_pin_slice2,frequency2);
        
//     }else if(gpio_get_irq_event_mask(GATE2) & GPIO_IRQ_EDGE_FALL){
//         gpio_acknowledge_irq(GATE2, GPIO_IRQ_EDGE_FALL );
//     }
// }
