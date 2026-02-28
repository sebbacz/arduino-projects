
#include "led_lib.h"

int main(void) {
    configure_leds();

    blink_led(PB2, 10); //  blink led 1  

    return 0;
}
