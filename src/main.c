// Sample code for ECE 198

// Written by Josh and Arman, November 2021

// This file contains code for a number of different examples.
// Each one is surrounded by an #ifdef ... #endif block inside of main().

// To run a particular example, you should remove the comment (//) in
// front of exactly ONE of the following lines:

//this is a comment to add change

//#define BUTTON_BLINK
// #define LIGHT_SCHEDULER
// #define TIME_RAND
// #define KEYPAD
// #define KEYPAD_CONTROL
// #define SEVEN_SEGMENT
// #define KEYPAD_SEVEN_SEGMENT
// #define COLOR_LED
// #define ROTARY_ENCODER
// #define ANALOG
// #define PWM


//We created these #ifdef ..... #endif blocks for testing
//The one uncommented is where the code for our game is
//#define TURN_ALL_LIGHTS_ON
//#define TURN_ON_RANDOM_LIGHTS
//#define CORRESPONDING_BUTTONS_TO_LIGHTS
#define MAIN_GAME

#include <stdbool.h> // booleans, i.e. true and false
#include <stdio.h>   // sprintf() function
#include <stdlib.h>  // srand() and random() functions

#include "ece198.h" //very important

//This is testing to make sure we r in the right repository on Nov 19
int main(void)
{
    HAL_Init(); // initialize the Hardware Abstraction Layer

    // Peripherals (including GPIOs) are disabled by default to save power, so we
    // use the Reset and Clock Control registers to enable the GPIO peripherals that we're using.

    __HAL_RCC_GPIOA_CLK_ENABLE(); // enable port A (for the on-board LED, for example)
    __HAL_RCC_GPIOB_CLK_ENABLE(); // enable port B (for the rotary encoder inputs, for example)
    __HAL_RCC_GPIOC_CLK_ENABLE(); // enable port C (for the on-board blue pushbutton, for example)

    // initialize the pins to be input, output, alternate function, etc...

    InitializePin(GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0); // on-board LED
    
    InitializePin(GPIOC, GPIO_PIN_0, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0); // breadboard LED 0
    InitializePin(GPIOC, GPIO_PIN_1, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0); // breadboard LED 1
    InitializePin(GPIOB, GPIO_PIN_0, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0); // breadboard LED 2
    InitializePin(GPIOA, GPIO_PIN_4, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0); // breadboard LED 3

    InitializePin(GPIOA, GPIO_PIN_8, GPIO_MODE_INPUT, GPIO_PULLDOWN, 0);   // BUTTON 0 PA3
    InitializePin(GPIOA, GPIO_PIN_9, GPIO_MODE_INPUT, GPIO_PULLDOWN, 0);   // BUTTON 1 PA2
    InitializePin(GPIOA, GPIO_PIN_10, GPIO_MODE_INPUT, GPIO_PULLDOWN, 0);   // BUTTON 2 PA10
    InitializePin(GPIOB, GPIO_PIN_3, GPIO_MODE_INPUT, GPIO_PULLDOWN, 0);   // BUTTON 3 PB3


    // note: the on-board pushbutton is fine with the default values (no internal pull-up resistor
    // is required, since there's one on the board)

    // set up for serial communication to the host computer
    // (anything we write to the serial port will appear in the terminal (i.e. serial monitor) in VSCode)

    SerialSetup(9600);

    // as mentioned above, only one of the following code sections will be used
    // (depending on which of the #define statements at the top of this file has been uncommented)

//This is where the code for our game is
#ifdef MAIN_GAME
    //variable for round number
    u_int32_t round_num = 10;
    
    //loop to rcontrol program runtime
    while (1)
    {

        //initialize 7 segment display
        Initialize7Segment();

        //keep display off
        Display7Segment(round_num);

        //if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 1) // when button is start the game
        //{
        while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)) 
            ;

        srand(HAL_GetTick()); // set the random seed to be the time in ms that it took to press the button

        //variable for the score
        u_int32_t score = 0;

        //start the round number at 0
        round_num = 0;

        //the length of the timer between when the light turns on and when it must be clicked
        u_int32_t timer_length = 5000;

        //the length of the delay between buttons
        u_int32_t between_lights_delay = 500;

        //value to decrease the timer between the light turning on 
        //and the button press each round
        u_int32_t const timer_dec_length = 50;

        //value to decrease the delay between the next light turning on
        //each round
        u_int32_t const delay_dec_length = 5;

        //count down the user with the 7 segment display
        for (int i = 5; i > 0; i--) {
            Display7Segment(i);
            HAL_Delay(1000);

        }

        //start the display at 0
        Display7Segment(0);

        //loop that controls main game
        while (true)
        {
            //timer variables to track elapsed time
            u_int32_t first_time = 0;
            u_int32_t second_time = 0;

            //variable to track if the correct button was clicked on time
            bool button_clicked = true;

            //call the random function and stores a number between 0 and 3
            u_int32_t rand_num = random() % 4;

            //store the time the light turns on
            first_time = HAL_GetTick();

            if (rand_num == 0)
            {
                //turn on fisrt light
                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, 1);

                //wait for the button to be pressed
                while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8) == 0){
                    //this time will increment
                    second_time = HAL_GetTick();

                    //if the elapsed time is greater than timer_length, or the wrong button was pressed
                    if ((second_time - first_time) >= timer_length || HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) == 1 || 
                        HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == 1 || HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) == 1) {

                        //then the button was not pressed
                        button_clicked = false;
                        break;
                    }
                }
            }
            else if (rand_num == 1)
            {
                //turn on second light
                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, 1);

                while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) == 0){
                    //this time will increment
                    second_time = HAL_GetTick();

                    //if the elapsed time is greater than timer_length, or the wrong button was pressed
                    if ((second_time - first_time) >= timer_length || HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8) == 1 || 
                        HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == 1 || HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) == 1) {
                        button_clicked = false;
                        break;
                    }
                }
            }
            else if (rand_num == 2)
            {
                //turn on third light
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1);
                while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == 0){
                    //this time will increment
                    second_time = HAL_GetTick();

                    //if the elapsed time is greater than timer_length, or the wrong button was pressed
                    if ((second_time - first_time) >= timer_length || HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8) == 1 || 
                        HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) == 1 || HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) == 1)  {
                        button_clicked = false;
                        break;
                    }
                }
            }
            else
            {
                //turn on fourth light
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 1);
                while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) == 0){
                    //this time will increment
                    second_time = HAL_GetTick();

                    //if the elapsed time is greater than timer_length, or the wrong button was pressed
                    if ((second_time - first_time) >= timer_length || HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8) == 1 || 
                        HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) == 1 || HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == 1) {
                        button_clicked = false;
                        break;
                    }
                }

            }

            //if all the buttons are being pressed at the same time
            if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8) == 1 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) == 1 &&
                HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == 1 && HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) == 1) {
                    button_clicked = false;
            }

            //then turn off all the lights
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, 0);
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, 0);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0);

            //if the timer length is ever less than equal to 0, then set it back to 50
            if (timer_length <= 0) {
                timer_length = 50;
            }

            //if the delay length is ever less than or equal to 0
            if (between_lights_delay <= 0) {
                between_lights_delay = 5;
            }
            //if the button was clicked
            if (button_clicked) {
                //then increase the score by 1
                score = score + 1;
                
                //calculate the round number based on the score
                round_num = score/10;

                //display the round number to the 7 segment display
                Display7Segment(round_num);

                //make the timer shorter
                timer_length = timer_length - timer_dec_length;

                //make the delay between buttons shorter
                between_lights_delay = between_lights_delay - delay_dec_length;

                //print the score to the console
                char buff[110];
                sprintf(buff, "\tlight: %lu \t score: %lu \t level: %lu \t timer: %lu\t delay: %lu \r \n", rand_num, score, round_num, timer_length, between_lights_delay);
                SerialPuts(buff);

                //turn on and off the green LED on the board
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
                HAL_Delay(100);
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);     

                //have more delay before the next light turns on
                HAL_Delay(between_lights_delay);
            }
            //if the button is not pressed
            else {
                char buff[100];
                sprintf(buff, "you lose!! \n \r");
                SerialPuts(buff);

                //the game is over and break out of the loop
                break;
            }
        }

        //blink the score and on-baord LED to signal that the user lost the game
        for (int i = 0; i < 10; i++) {
            HAL_Delay(100);
            Display7Segment(10);
            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
            HAL_Delay(100);
            Display7Segment(round_num);
            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        }

    }
#endif

#ifdef CORRESPONDING_BUTTONS_TO_LIGHTS
    //loop to repeat forever
    while (1){
        //if (button 0 is pressed)
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8) == 1){
            //then turn on light one
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, 1);
        }

        //else if button 1 is pressed
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) == 1) {
            //then turn on light two
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1,1);
        }
        
            
        //else if button 2 is pressed
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == 1){
            //then turn on light three
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1);
        }

        if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) == 1){
        //else if button 3 is pressed
            //then turn on light fourth
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 1);
        }
        
    }

#endif

#ifdef TURN_ON_RANDOM_LIGHTS
    //loop to repeat
    while (1)
    {
        //if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 1) // when button is pressed toggle through lights
        //{
        while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13))
            ;
        // wait for button press
        srand(HAL_GetTick()); // set the random seed to be the time in ms that it took to press the button

        //repeat the random numbers turning on and off
        while (true)
        {
            //call the random function
            u_int32_t rand_num = random() % 4;

            if (rand_num == 0)
            {
                //turn on fisrt light
                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, 1);
            }
            else if (rand_num == 1)
            {
                //turn on second light
                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, 1);
            }
            else if (rand_num == 2)
            {
                //turn on third light
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1);
            }
            else
            {
                //turn on fourth light
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 1);
            }
            //wait a second
            HAL_Delay(1000);

            //then turn off all the lights
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, 0);
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, 0);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0);
            HAL_Delay(1000);
        }

        //}
    }
#endif

#ifdef TURN_ALL_LIGHTS_ON
    while (1)
    {
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8) == 1) // when button is pressed toggle through lights
        {

            HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_0);
            HAL_Delay(250);
            HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_1);
            HAL_Delay(250);
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
            HAL_Delay(250);
            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_4);
            HAL_Delay(250);
        }
        else
        {
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, 0);
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, 0);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0);
        }
    }

    // while (1)
    // {
    //     HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_0);
    //     HAL_Delay(250);
    //     HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_1);
    //     HAL_Delay(250);
    //     HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
    //     HAL_Delay(250);
    //     HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_4);
    //     HAL_Delay(250);
    // }
    // HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, 1);
    // HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, 1);
    // HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1);
    // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 1);

#endif

#ifdef BUTTON_BLINK
    // Wait for the user to push the blue button, then blink the LED.

    // wait for button press (active low)
    while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13))
    {
    }

    while (1) // loop forever, blinking the LED
    {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        HAL_Delay(250); // 250 milliseconds == 1/4 second
    }
#endif

#ifdef LIGHT_SCHEDULER
    // Turn on the LED five seconds after reset, and turn it off again five seconds later.

    while (true)
    {
        uint32_t now = HAL_GetTick();
        if (now > 5000 && now < 10000)
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, true); // turn on LED
        else
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, false); // turn off LED
    }
#endif

#ifdef TIME_RAND
    // This illustrates the use of HAL_GetTick() to get the current time,
    // plus the use of random() for random number generation.

    // Note that you must have "#include <stdlib.h>"" at the top of your main.c
    // in order to use the srand() and random() functions.
    char buff[100];
    sprintf(buff, "something to print");
    SerialPuts(buff);

    while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13))
        ;                 // wait for button press
    srand(HAL_GetTick()); // set the random seed to be the time in ms that it took to press the button
    // if the line above is commented out, your program will get the same sequence of random numbers
    // every time you run it (which may be useful in some cases)

    while (true) // loop forever
    {
        while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13))
            ; // wait for button press, it goes low when pressed

        // Display the time in milliseconds along with a random number.
        // We use the sprintf() function to put the formatted output into a buffer;
        // see https://www.tutorialspoint.com/c_standard_library/c_function_sprintf.htm for more
        // information about this function
        char buff[100];

        //this will generate a random number between 0 and 3
        // u_int32_t rand_num = random()%4;

        sprintf(buff, "Time: %lu ms   Random = %ld\r\n", HAL_GetTick(), random());
        // lu == "long unsigned", ld = "long decimal", where "long" is 32 bit and "decimal" implies signed
        SerialPuts(buff); // transmit the buffer to the host computer's serial monitor in VSCode/PlatformIO

        while (!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13))
            ; // wait for button to be released
    }
#endif

#ifdef KEYPAD
    // Read buttons on the keypad and display them on the console.

    // this string contains the symbols on the external keypad
    // (they may be different for different keypads)
    char *keypad_symbols = "123A456B789C*0#D";
    // note that they're numbered from left to right and top to bottom, like reading words on a page

    InitializeKeypad();
    while (true)
    {
        while (ReadKeypad() < 0)
            ;                                     // wait for a valid key
        SerialPutc(keypad_symbols[ReadKeypad()]); // look up its ASCII symbol and send it to the hsot
        while (ReadKeypad() >= 0)
            ; // wait until key is released
    }
#endif

#ifdef KEYPAD_CONTROL
    // Use top-right button on 4x4 keypad (typically 'A') to toggle LED.

    InitializeKeypad();
    while (true)
    {
        while (ReadKeypad() < 0)
            ; // wait for a valid key
        int key = ReadKeypad();
        if (key == 3)                              // top-right key in a 4x4 keypad, usually 'A'
            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5); // toggle LED on or off
        while (ReadKeypad() >= 0)
            ; // wait until key is released
    }
#endif

#ifdef SEVEN_SEGMENT
    // Display the numbers 0 to 9 inclusive on the 7-segment display, pausing for a second between each one.
    // (remember that the GND connection on the display must go through a 220 ohm current-limiting resistor!)

    Initialize7Segment();
    while (true)
        for (int i = 0; i < 10; ++i)
        {
            Display7Segment(i);
            HAL_Delay(1000); // 1000 milliseconds == 1 second
        }
#endif

#ifdef KEYPAD_SEVEN_SEGMENT
    // Combines the previous two examples, displaying numbers from the keypad on the 7-segment display.

    // this string contains the symbols on the external keypad
    // (they may be different for different keypads)
    char *keypad_symbols = "123A456B789C*0#D";
    // note that they're numbered from left to right and top to bottom, like reading words on a page

    InitializeKeypad();
    Initialize7Segment();
    while (true)
    {
        int key = ReadKeypad();
        if (key >= 0)
            Display7Segment(keypad_symbols[key] - '0'); // tricky code to convert ASCII digit to a number
    }
#endif

#ifdef COLOR_LED
    // Cycle through all 8 possible colors (including off and white) as the on-board button is pressed.
    // This example assumes that the color LED is connected to pins D11, D12 and D13.

    // Remember that each of those three pins must go through a 220 ohm current-limiting resistor!
    // Also remember that the longest pin on the LED should be hooked up to GND.

    InitializePin(GPIOA, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0); // initialize color LED output pins
    while (true)
    {
        for (int color = 0; color < 8; ++color)
        {
            // bottom three bits indicate which of the three LEDs should be on (eight possible combinations)
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, color & 0x01); // blue  (hex 1 == 0001 binary)
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, color & 0x02); // green (hex 2 == 0010 binary)
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, color & 0x04); // red   (hex 4 == 0100 binary)

            while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13))
                ; // wait for button press
            while (!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13))
                ; // wait for button release
        }
    }
#endif

#ifdef ROTARY_ENCODER
    // Read values from the rotary encoder and update a count, which is displayed in the console.

    InitializePin(GPIOB, GPIO_PIN_5, GPIO_MODE_INPUT, GPIO_PULLUP, 0);  // initialize CLK pin
    InitializePin(GPIOB, GPIO_PIN_4, GPIO_MODE_INPUT, GPIO_PULLUP, 0);  // initialize DT pin
    InitializePin(GPIOB, GPIO_PIN_10, GPIO_MODE_INPUT, GPIO_PULLUP, 0); // initialize SW pin

    bool previousClk = false; // needed by ReadEncoder() to store the previous state of the CLK pin
    int count = 0;            // this gets incremented or decremented as we rotate the encoder

    while (true)
    {
        int delta = ReadEncoder(GPIOB, GPIO_PIN_5, GPIOB, GPIO_PIN_4, &previousClk); // update the count by -1, 0 or +1
        if (delta != 0)
        {
            count += delta;
            char buff[100];
            sprintf(buff, "%d  \r", count);
            SerialPuts(buff);
        }
        bool sw = !HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10); // read the push-switch on the encoder (active low, so we invert it using !)
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, sw);        // turn on LED when encoder switch is pushed in
    }
#endif

#ifdef ANALOG
    // Use the ADC (Analog to Digital Converter) to read voltage values from two pins.

    __HAL_RCC_ADC1_CLK_ENABLE();       // enable ADC 1
    ADC_HandleTypeDef adcInstance;     // this variable stores an instance of the ADC
    InitializeADC(&adcInstance, ADC1); // initialize the ADC instance
    // Enables the input pins
    // (on this board, pin A0 is connected to channel 0 of ADC1, and A1 is connected to channel 1 of ADC1)
    InitializePin(GPIOA, GPIO_PIN_0 | GPIO_PIN_1, GPIO_MODE_ANALOG, GPIO_NOPULL, 0);
    while (true)
    {
        // read the ADC values (0 -> 0V, 2^12 -> 3.3V)
        uint16_t raw0 = ReadADC(&adcInstance, ADC_CHANNEL_0);
        uint16_t raw1 = ReadADC(&adcInstance, ADC_CHANNEL_1);

        // print the ADC values
        char buff[100];
        sprintf(buff, "Channel0: %hu, Channel1: %hu\r\n", raw0, raw1); // hu == "unsigned short" (16 bit)
        SerialPuts(buff);
    }
#endif

#ifdef PWM
    // Use Pulse Width Modulation to fade the LED in and out.
    uint16_t period = 100, prescale = 16;

    __TIM2_CLK_ENABLE();                                           // enable timer 2
    TIM_HandleTypeDef pwmTimerInstance;                            // this variable stores an instance of the timer
    InitializePWMTimer(&pwmTimerInstance, TIM2, period, prescale); // initialize the timer instance
    InitializePWMChannel(&pwmTimerInstance, TIM_CHANNEL_1);        // initialize one channel (can use others for motors, etc)

    InitializePin(GPIOA, GPIO_PIN_5, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_AF1_TIM2); // connect the LED to the timer output

    while (true)
    {
        // fade the LED in by slowly increasing the duty cycle
        for (uint32_t i = 0; i < period; ++i)
        {
            SetPWMDutyCycle(&pwmTimerInstance, TIM_CHANNEL_1, i);
            HAL_Delay(5);
        }
        // fade the LED out by slowly decreasing the duty cycle
        for (uint32_t i = period; i > 0; --i)
        {
            SetPWMDutyCycle(&pwmTimerInstance, TIM_CHANNEL_1, i);
            HAL_Delay(5);
        }
    }
#endif
    return 0;
}

// This function is called by the HAL once every millisecond
void SysTick_Handler(void)
{
    HAL_IncTick(); // tell HAL that a new tick has happened
    // we can do other things in here too if we need to, but be careful
}
