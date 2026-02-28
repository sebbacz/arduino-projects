#include <avr/io.h>
#include <util/delay.h>
#include "display.h"

void initADC(){

    //input
    ADMUX = _BV(REFS0) | _BV(ADLAR) | POT_PIN;
}


void readADC(){

    //reead
    ADCSRA |= _BV(ADSC);
    while (ADCSRA & _BV(ADSC));

    return ADCH;

}


int main(){
    initADC();
    initDisplay();

    while (1)
    {
        int value = readADC();

        int vol = (value * 5) / 255;

        writeNumber(vol);
        _delay_ms(1000);
    }
    

    
    

    return 0;
}