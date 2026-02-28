#include <avr/io.h>
#include <util/delay.h>
#include "led_lib.h"

#define __DELAY_BACKWARD_COMPATIBLE__
#include <stdlib.h>

#define MIN_DELAY 200
#define MAX_DELAY 2000

void deleyms(int ms){
    for (int i = 0; i < ms; i++)
    {
        _delay_ms(1);
    }
}

int main(void){
    //initelize random
    srand(0);


    //enable leds
    enableAllLeds();


    while (1)
    {   

        // generate random led
        int ledNumber = rand() % 4 + 1;

        //generate random delay
         int delay = rand() % (MAX_DELAY - MIN_DELAY + 1) + MIN_DELAY;


         //use macro
         uint8_t led;
          switch (ledNumber) {
            case 1: led = LED1; break;
            case 2: led = LED2; break;
            case 3: led = LED3; break;
            case 4: led = LED4; break;


            // Light up the random LED for a random duration
        lightUpOneLed(led);
        delay_ms(delay);

        // Turn off the LED
        lightDownOneLed(led);
        delay_ms(delay);

        }


    }
    return 0;
}   
