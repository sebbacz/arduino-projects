#include <avr/io.h>
#include <util/delay.h>
#include "led_lib.h"

int main(void) {
    configure_leds();

    for (int delay_time = 10; delay_time <= 1000; delay_time += 50) {
        PORTB &= ~_BV(PB2); // Turn on LED1
        _delay_ms(delay_time);
        PORTB |= _BV(PB2);  // Turn off LED1
        _delay_ms(500);
    }

    return 0;
}
