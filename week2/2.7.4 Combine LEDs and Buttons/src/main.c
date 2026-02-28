#include <avr/io.h>
#include <avr/interrupt.h>
#include "BTN_LIB.h"
#include "LED_LIB.h"

void setup() {
  configure_leds();               // Configure the LEDs
  enableButton(0);                // Enable button 0
  enableButton(1);                // Enable button 1
  enableButton(2);                // Enable button 2
  enableAllButtonInterrupts();    // Enable interrupts for all buttons
  sei();                          // Enable global interrupts
}

void loop() {
  // Check button 0
  if (buttonPushed(0)) {
    lightUpLed(0);                 // Toggle LED 0
    while (!buttonReleased(0));   // Wait until the button is released
  }

  // Check button 1
  if (buttonPushed(1)) {
    lightUpLed(1);                 // Toggle LED 1
    while (!buttonReleased(1));   // Wait until the button is released
  }

  // Check button 2
  if (buttonPushed(2)) {
    lightUpLed(2);                 // Toggle LED 2
    while (!buttonReleased(2));   // Wait until the button is released
  }
}

int main() {
  setup();
  while (1) {
    loop();
  }
  return 0;
}