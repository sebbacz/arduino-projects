#include <avr/io.h>
#include <util/delay.h>
#include "led_lib.h"

int main(void) {
    configure_leds();

    for (int i = 0; i < 10; i++) {
        PORTB &= ~_BV(PB2); // turn on LED1
        _delay_ms(500);
        PORTB |= _BV(PB2);  // turn off LED1
        _delay_ms(500);
    }

    return 0;
}
