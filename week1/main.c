#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#include "led_lib.h"

int main() 
{
    fadeInLed(1, 2000); // led1 2sec
    _delay_ms(2000); // w8 2 sec
    fadeOutLed(1, 2000); // led 1 two sec
    return 0;
}
