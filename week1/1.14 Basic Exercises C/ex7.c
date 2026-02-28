#include <avr/io.h>
#include <util/delay.h>
#include "led_lib.h"

int main(void) {
    configure_leds();

    char sequence[] = "abcdabcd";

    for (int i = 0; sequence[i] != '\0'; i++) {
        switch (sequence[i]) {
            case 'a':
                PORTB &= ~_BV(PB2); // Turn on LED1
                _delay_ms(500);
                PORTB |= _BV(PB2);  // Turn off LED1
                break;
            case 'b':
                PORTB &= ~_BV(PB3); // Turn on LED2
                _delay_ms(500);
                PORTB |= _BV(PB3);  // Turn off LED2
                break;
            case 'c':
                PORTB &= ~_BV(PB4); // Turn on LED3
                _delay_ms(500);
                PORTB |= _BV(PB4);  // Turn off LED3
                break;
            case 'd':
                PORTB &= ~_BV(PB5); // Turn on LED4
                _delay_ms(500);
                PORTB |= _BV(PB5);  // Turn off LED4
                break;
        }
        _delay_ms(500);
    }

    return 0;
}
