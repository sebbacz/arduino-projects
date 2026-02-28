#include <avr/io.h>
#include <util/delay.h>
#include "led_lib.h"

int main(void) {
    configure_leds();

    int led_array[10] = {0, 1, 2, 3, 0, 1, 2, 3, 0, 1};

    for (int i = 0; i < 10; i++) {
        uint8_t led_mask = _BV(PB2 + led_array[i]);
        PORTB &= ~led_mask; 
        _delay_ms(500);
        PORTB |= led_mask;  // turn off  
        _delay_ms(500);
    }

    return 0;
}
