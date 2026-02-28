// PB2 = LED1
// PB3 = LED2
// PB4 = LED3
// PB5 = LED4

#include <stdint.h>

void configure_leds(void);
void all_led_on(void);
void all_led_off(void);
void plan1(void);
void plan2(void);
void plan3(void);

void lightToggleOneLed(int);
void lightUpOneLed(int); 
void lightDownOneLed(int); 

void enableLed(int ledNumber);
void enableLeds(uint8_t leds);
void enableAllLeds();
void lightUpLed(int ledNumber);
void lightUpLeds(uint8_t leds);
void lightUpAllLeds();
void lightDownLed(int ledNumber);
void lightDownLeds(uint8_t leds);
void lightDownAllLeds();
void dimLed(int ledNumber, int percentage, int duration);
void fadeInLed(int ledNumber, int duration);
