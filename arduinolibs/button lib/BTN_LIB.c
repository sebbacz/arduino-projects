#include <avr/io.h>
#include "BTN_LIB.h"

uint8_t btn_state[3] = {1,1,1};

void enableButton(int button) {
  // DDRx – Data Direction Register
  // PINx – Pin Input Register
  // PORTx – Pin Output Register
  // SW1 = PC1
  // SW2 = PC2
  // SW3 = PC3
  // button value will be 1 to 3
  DDRC &= _BV(button);
  PCMSK1 |= _BV(button);  
}

void save_btn_state(uint8_t * btn) {
  btn_state[0] = btn[0];
  btn_state[1] = btn[1];
  btn_state[2] = btn[2];  
}

int buttonPushed(int button) {
  // when button is pressed pin is at low level
  // and function return 1 otherwise 0
  if(btn_state[button-1]) 
  {
    return 0;
  }
  return 1;
}

int buttonReleased(int button) {
  // when button is released pin is at high level
  // and function return 1 otherwise 0
  if(btn_state[button-1])
  {
    return 1;
  }
  return 0;
}


void enableButtonInterrupt(int button) {
  PCICR |= _BV(PCIE1);            // enable pin change interrupt  
  PCMSK1 |= _BV(button + 1);      // enable interrupt for the specific button pin
}

void enableAllButtonInterrupts(void) {
  PCICR |= _BV(PCIE1);            // E 
  PCMSK1 |= _BV(PC1) | _BV(PC2) | _BV(PC3);  
}