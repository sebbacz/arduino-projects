#include <avr/io.h>
#include <util/delay.h>
#include "display_lib.h"

// Bytes map for numbers from null to nine and special characters
const uint8_t SEGMENT_MAP[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99,
                               0x92, 0x82, 0xF8, 0x80, 0x90};

// Digits 1 2 3 4
const uint8_t SEGMENT_SELECT[] = {0xF1, 0xF2, 0xF4, 0xF8};

// Function to initialize the display pins
void initDisplay() {
    DDRD |= (1 << LATCH_DIO) | (1 << CLK_DIO);
    DDRB |= (1 << DATA_DIO);
}

// Function to shift out a byte of data to the display
void shift(uint8_t val, uint8_t bitorder) {
    uint8_t bit;
    for (uint8_t i = 0; i < 8; i++) {
        if (bitorder == LSBFIRST) {
            bit = !!(val & (1 << i));
        } else {
            bit = !!(val & (1 << (7 - i)));
        }
        if (bit) {
            PORTB |= (1 << DATA_DIO);
        } else {
            PORTB &= ~(1 << DATA_DIO);
        }
        PORTD |= (1 << CLK_DIO);
        PORTD &= ~(1 << CLK_DIO);
    }
}

// Function to write a number to a specific segment of the display
void writeNumberToSegment(uint8_t segment, uint8_t value) {
    PORTD &= ~(1 << LATCH_DIO);
    shift(SEGMENT_MAP[value], MSBFIRST);
    shift(SEGMENT_SELECT[segment], MSBFIRST);
    PORTD |= (1 << LATCH_DIO);
}

// Function to write a scrolling number to the display
void writeScrollingNumber(uint8_t start) {
    for (uint8_t i = 0; i < 4; i++) {
        writeNumberToSegment(i, (start + i) % 10);
    }
}

int main(void) {
    initDisplay();

    uint8_t start = 1; // Start with '1234'

    while (1) {
        writeScrollingNumber(start);
        start = (start + 1) % 10;
        _delay_ms(1000);
    }

    return 0;
}
