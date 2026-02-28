
#include "LED_LIB.h"

#include <avr/io.h>
#include <util/delay.h>

#define __DELAY_BACKWARD_COMPATIBLE__



// Function to configure the LEDs
void configure_leds(void) {
  all_led_off(); // Turn off all LEDs
  DDRB |= _BV(PB5) | _BV(PB4) | _BV(PB3) | _BV(PB2); // Set PB5, PB4, PB3, PB2 as output pins
}


void enableLed(int ledNumber)
{
    if (ledNumber < 0 || ledNumber > 3)
        return; // Check if LED number is within range
    DDRB |= (1 << (PB2 + ledNumber)); // Set the respective pin as output (1) for the specified LED
}

// Function to turn on all LEDs
void all_led_on(void) {
  PORTB &= ~(_BV(PB5) | _BV(PB4) | _BV(PB3) | _BV(PB2)); // Clear PB5, PB4, PB3, PB2 to turn on the LEDs
}

// Function to turn off all LEDs
void all_led_off(void) {
  PORTB |= _BV(PB5) | _BV(PB4) | _BV(PB3) | _BV(PB2); // Set PB5, PB4, PB3, PB2 to turn off the LEDs
}

// Function for plan 1
void plan1(void) {
  PORTB &= ~_BV(PB2); // Clear PB2 to activate plan 1
}

// Function for plan 2
void plan2(void) {
  PORTB &= ~(_BV(PB3) | _BV(PB2)); // Clear PB3 and PB2 to activate plan 2
}

// Function for plan 3
void plan3(void) {
  PORTB &= ~(_BV(PB4) | _BV(PB3) | _BV(PB2)); // Clear PB4, PB3, and PB2 to activate plan 3
}


// Function to turn on a specific LED based on the LED number
void lightUpLed(int ledNumber)
{
    if (ledNumber < 0 || ledNumber > 3)
        return; // Check if LED number is within range
    PORTB &= ~(1 << (PB2 + ledNumber)); // Set the respective pin low (0) to turn on the LED
}


void lightUpOneLed(int led){
    if ( led < 0 || led > 3 ) return;
    PORTB &= ~(1 << (led+2));
}


// Function to enable a combination of LEDs based on the given byte
void enableLeds(uint8_t leds)
{
    DDRB |= leds; // Set the respective pins as output for the specified LEDs
}

// Function to enable all LEDs
void enableAllLeds()
{
    DDRB |= 0b00111100; // Set all pins of the LED output as output
}


void lightDownOneLed(int led){
    if ( led < 0 || led > 3 ) return;
    PORTB |= (1 << (led+2));
}

void lightToggleOneLed(int led){
    PORTB ^= (0 << (led+2));
}

// Function to turn on a combination of LEDs based on the given byte
void lightUpLeds(uint8_t leds)
{
    PORTB &= ~leds; // Set the respective pins low (0) to turn on the specified LEDs
}

// Function to turn on all LEDs
void lightUpAllLeds()
{
    PORTB &= ~(0b00111100); // Set all pins of the LED output low (0) to turn on all LEDs
}

// Function to turn off a specific LED based on the LED number
void lightDownLed(int ledNumber)
{
    if (ledNumber < 0 || ledNumber > 3)
        return; // Check if LED number is within range
    PORTB |= (1 << (PB2 + ledNumber)); // Set the respective pin high (1) to turn off the LED
}

// Function to turn off a combination of LEDs based on the given byte
void lightDownLeds(uint8_t leds)
{
    PORTB |= leds; // Set the respective pins high (1) to turn off the specified LEDs
}

// Function to turn off all LEDs
void lightDownAllLeds()
{
    PORTB |= 0b00111100; // Set all pins of the LED output high (1) to turn off all LEDs
}

// Add a function dimLed(int ledNumber, int percentage, int duration) to the led library that makes the led dimmed
// for the given duration (in milliseconds) to the given percentage

void dimLed(int ledNumber, int percentage, int duration)
{
    enableLed(ledNumber);
    int interval = 500;
    percentage = percentage % 100;
    int freq = 100 / percentage;
    for (int i = 0; i < interval; i++)
    {
        if (i % freq == 0)
        {
            lightUpLed(ledNumber);
        }
        else
        {
            lightDownLed(ledNumber);
        }
        _delay_ms(duration / interval);
    }
}

// Add a function fadeInLed(int ledNumber, int duration) that uses the dimLed function to "fade in" a led
// from 0 to 100% over a period of duration milliseconds.

void fadeInLed(int ledNumber, int duration)
{
    // Loop through percentages from 0 to 100 and dim the LED accordingly
    for (int percentage = 0; percentage <= 100; percentage++)
    {
        dimLed(ledNumber, percentage, duration / 100);
    }
}

