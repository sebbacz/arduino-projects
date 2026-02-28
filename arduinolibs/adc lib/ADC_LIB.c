#include <util/delay.h>
#include <avr/io.h>
#include "usart.h"
#include "DISPLAY_LIB.h"

void startADC(void) {
   ADCSRA |= _BV(ADSC) | _BV(ADIE);
}

void initADC(void) {
  ADMUX = (1 << REFS0);    // Set up of reference voltage.
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
  ADCSRA |= (1 << ADEN );   // Enable the ADC
  ADCSRA |= (1 << ADIE);
  ADCSRA |= (1 << ADATE);
  ADCSRB = 0;
}

uint8_t mapADC(uint16_t value) {

  uint8_t mappedValue = (230UL*value)/1023UL + 10UL;  // Map the ADC value to a desired range (10-240)
  return  mappedValue;
}
