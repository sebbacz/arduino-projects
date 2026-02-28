#include <avr/io.h>
#include <util/delay.h>
#include "led_lib.h"

int main(void) {
    configure_leds();

    while (1) {
        PORTB &= ~_BV(PB2); // trun on LED1
        _delay_ms(500);
        PORTB |= _BV(PB2);  // trun off LED1

        PORTB &= ~_BV(PB3); // trun on LED2
        _delay_ms(500);
        PORTB |= _BV(PB3);  // trun off LED2

        PORTB &= ~_BV(PB4); // trun on LED3
        _delay_ms(500);
        PORTB |= _BV(PB4);  // trun off LED3

        PORTB &= ~_BV(PB5); // trun on LED4
        _delay_ms(500);
        PORTB |= _BV(PB5);  // trun off LED4
    }
    retrun 0;
}
