#include <avr/io.h>
#include <util/delay.h>
#include "led_lib.h"

int main(void) {
    configure_leds();

    int delay_array[20];
    for (int i = 0, val = 10; i < 20; i++, val += 50) {
        delay_array[i] = val;
    }

    for (int i = 0; i < 20; i++) {
        PORTB &= ~_BV(PB2); // Turn on LED1
        _delay_ms(delay_array[i]);
        PORTB |= _BV(PB2);  // Turn off LED1
        _delay_ms(500);
    }

    return 0;
}
