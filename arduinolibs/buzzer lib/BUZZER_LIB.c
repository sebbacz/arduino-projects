#include <avr/io.h>
#include "BUZZER_LIB.h"
#define __DELAY_BACKWARD_COMPATIBLE__ 
#include <stdio.h>
#include <util/delay.h>

void configure_buzzer(void) {
  DDRD |= _BV(PD3);       // buzzer pin as a output
  TCCR0B = 0x00;
  TCCR0A = _BV(WGM01);    // CTC mode
  TIMSK0 |= _BV(OCIE0A);
  PORTD |=_BV(PD3);       // off the buzzer    
}

void set_beep_freq (uint16_t freq) {
  // Timer 0 is a 8 bit timer 
  switch(freq) {
    case 50:
 
      OCR0A  = 156;                      
      // start Timer 2
      TCCR0B =  _BV (CS02) | _BV (CS00) ;   
    break;

    case 100:
 
      OCR0A  = 77;                          
      // start Timer 2
      TCCR0B =  _BV (CS02) | _BV (CS00) ;   
    break;
    
    case 500:
   
      OCR0A  = 249;                        
      // start Timer 2
      TCCR0B = _BV (CS01) | _BV (CS00) ;    
    break;

    case 1000:
      // 1000 Hz setting
      OCR0A  = 124;                       
      // start Timer 2
      TCCR0B = _BV (CS01) | _BV (CS00) ;    
    break;

    case 2000:
      // 2000 Hz setting
      OCR0A  = 62;                        
      // start Timer 2
      TCCR0B = _BV (CS01) | _BV (CS00) ;   
    break;

    case 4000:
      // 4000 Hz setting
      OCR0A  = 249;                       
      // start Timer 2
      TCCR0B = _BV (CS01) ;                
    break; 

    default:
      TCCR0B = 0x00;                        // no clock
      PORTD |= _BV(PD3);                    // off the buzzer
    break;
  }
}


//
void enableBuzzer(){
    DDRD |= 0b00001000;
}

void buzzerOn(){
    PORTD &= ~(1 << PD3);
}




void playBuzz( float frequency, uint32_t duration )
{
   uint32_t periodInMicro = ( uint32_t ) ( 1000000 / frequency ); //Calculate the period in microsecs from the freq
   uint32_t durationInMicro = duration * 1000; //We express duration in microsecs
   for ( uint32_t time = 0; time < durationInMicro; time += periodInMicro ) //See tutorial on Music for more info!
    {
    PORTD &= ~( 1 << PD3 ); //turn the buzzer on
    _delay_us( periodInMicro / 2 ); //Wait for the half of the period
    PORTD |= ( 1 << PD3 ); //Turn the buzzer off
    _delay_us( periodInMicro / 2 ); //Wait again for half of the period
    }
}

void endBuzz(){
  PORTD |= ( 1 << PD3 );
}


