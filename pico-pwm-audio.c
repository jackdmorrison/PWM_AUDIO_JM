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
//global variables for Audio channels 
uint audio_pin_channel;


//global wave postion variables
int wav_position = 0;

//global frequency variables for each Output
float frequency;

//currentF used to determine frequency in vibrato
float currentF ;

//buttonNum used to determine level input array 
int buttonNum = 0;

//used to determine bitshift for each octave to increase number of repetitions of level array values
int val=1;

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
        val=0;
    }
    //Octave 4
    else if(newFrequency>freqListJust[32]){
        //find clock new clock divider
        clockDiv=(WAV_FREQUENCY/newFrequency)*clkDiv*2;
        //find the bitshift value 
        val=1;
        
    }
    //Octave 1
    else if(newFrequency<freqListJust[12]){
        //find clock new clock divider
        clockDiv=(WAV_FREQUENCY/newFrequency)*clkDiv/4;
        //find the bitshift value 
        val=4;
        
    }
    //Octave 2
    else if(newFrequency<freqListJust[24]){
        //find clock new clock divider
        clockDiv=(WAV_FREQUENCY/newFrequency)*clkDiv/2;
        val=3;
    }
    //Octave 3
    else{
        //find clock new clock divider
        clockDiv=(WAV_FREQUENCY/newFrequency)*clkDiv;
        //find the bitshift value 
        val=2;
        
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
float findValue(int wave_position){
    //value variable used to determine current level
    int value=0;
    value=SIN_WAV_DATA[wave_position]+HARMONIC2_WAV_DATA[wave_position]+HARMONIC3_WAV_DATA[wave_position]+HARMONIC4_WAV_DATA[wave_position]+HARMONIC5_WAV_DATA[wave_position]+HARMONIC6_WAV_DATA[wave_position]+HARMONIC7_WAV_DATA[wave_position]+HARMONIC8_WAV_DATA[wave_position];//1
    //return level needed to achieve effect parameters
    return value/8;
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
    //check pin slice 
        pwm_clear_irq(audio_pin_slice);
        /*
            if the wave postion is less than the wave position
            variable bit shift that modulates the number of repeated 
            values at each octave
        */
        if (wav_position < (WAV_DATA_LENGTH<<val) - 1) { 
            // set pwm level on slice and pin
            // allow the pwm value to repeat for 2^val cycles  
            level=findValue(wav_position>>val);
            pwm_set_chan_level(audio_pin_slice,audio_pin_channel, round(level) );
            wav_position++;
        } else {
            // reset to start
            wav_position = 0;

            
        }
}

int main(void) {
    //set initial frequency
    frequency=freqListEqualT[0];

    //initialise LED Pin
    gpio_init(LED);
    gpio_set_dir(LED, GPIO_OUT);
    gpio_put(LED, true);

    stdio_init_all();

    
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
    irq_set_enabled(PWM_IRQ_WRAP, true);

    while(1) {
        __wfi(); // Wait for Interrupt
    }
}