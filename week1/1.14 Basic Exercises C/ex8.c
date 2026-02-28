#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "led_lib.h"

int main(void) {
    configure_leds();

    char sequence[] = "abcdabcd"; // random

    for (int i = 0; sequence[i] != '\0'; i++) {
        int flashes = sequence[i] - 'a' + 1;
        for (int j = 0; j < flashes; j++) {
            all_leds_on();
            _delay_ms(500);
            all_leds_off();
            _delay_ms(500);
        }
        _delay_ms(1000); // deley
    }

    return 0;
}
