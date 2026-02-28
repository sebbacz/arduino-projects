#define __DELAY_BACKWARD_COMPATIBLE__

#include <usart.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

// Include external libraries
#include <DISPLAY_LIB.h>
#include <BUZZER_LIB.h>
#include <BTN_LIB.h>
#include <LED_LIB.h>

// Constants
const float GRAVITY = 1.622; // m/s²
const int INITIAL_FUEL = 1500; // liters
const int TIMER0_PRESCALER = (1 << CS02) | (1 << CS00); // 64
const int TIMER1_PRESCALER = (1 << CS12); // 256
const int TIMER1_PERIOD = 62500; // 1 second in timer ticks
const int PWM_DUTY_CYCLE = 156;

// Data Log Structure
typedef struct {
    int distance;
    int explosion;
    int setSpeed;
    int fuel;
} DataLog;

volatile int distance = 9999; // meters
volatile int fuel;
volatile int explosion = 0;
volatile int ledCounter = 0;
volatile int leds = 0;
volatile int grSpeed = 1;
volatile float setSpeed = 100; // m/s

DataLog* output[500]; // Storage for output logs
int logIndex = 0; // Index for output logs

// Function Declarations
void initTimer0(void);
void initTimer1(void);
void showParameter(void);

// Timer 0 Initialization
void initTimer0(void) {
    TCCR0A |= (1 << WGM00); // Fast PWM mode
    TCCR0B |= (1 << WGM02); 
    TCCR0B |= TIMER0_PRESCALER; // Set prescaler
    OCR0A = PWM_DUTY_CYCLE; // Set PWM duty cycle
    TIMSK0 |= (1 << TOIE0); // Enable overflow interrupt
}

// Timer 1 Initialization
void initTimer1(void) {
    TCCR1A |= (1 << WGM10) | (1 << WGM11); // Fast PWM mode
    TCCR1B |= (1 << WGM12) | (1 << WGM13); 
    TCCR1B |= TIMER1_PRESCALER; // Set prescaler
    OCR1A = TIMER1_PERIOD; // Set timer period
    TIMSK1 |= (1 << TOIE1); // Enable overflow interrupt
}

// Display Parameters
void showParameter(void) {
    writeNumber(distance);
}

// Timer 1 Overflow Interrupt Service Routine
ISR(TIMER1_OVF_vect) {
    setSpeed += GRAVITY - explosion / 5.0;
    distance -= (int)setSpeed;
    fuel -= explosion;

    // Log data
    DataLog* logEntry = malloc(sizeof(DataLog));
    if (logEntry) {
        logEntry->distance = distance;
        logEntry->setSpeed = setSpeed;
        logEntry->fuel = fuel;
        logEntry->explosion = explosion;
        output[logIndex++] = logEntry;
    }

    // Update display speed and LED state
    grSpeed = (fuel % 375) / 18;
    leds = fuel / 375;
    explosion = 0;
}

// Timer 0 Overflow Interrupt Service Routine
ISR(TIMER0_OVF_vect) {
    ledCounter++;
    if (ledCounter >= grSpeed) {
        lightToggleOneLed(leds + 1);
        ledCounter = 0;
        lightDownOneLed(leds + 2);
    }
    
    if (buttonPushed(1) && (fuel - explosion > 0)) {
        if (explosion <= 50) {
            explosion++;
        }
    } else if (fuel <= 0) {
        lightDownAllLeds();
        TCCR0B &= ~TIMER0_PRESCALER; // Stop timer 0
        playBuzz(NOTE_C5, 90);
        endBuzz();
        _delay_ms(10);
        playBuzz(NOTE_G5, 90);
        endBuzz();
        _delay_ms(10);
        playBuzz(NOTE_C5, 90);
        endBuzz();
        _delay_ms(10);
        playBuzz(NOTE_C5, 90);
        endBuzz();
    }
}

// Main Function
int main(void) {
    enableAllLeds();
    initDisplay();
    lightUpAllLeds();
    enableBuzzer();
    initUSART();
    enableButton(1);
    initTimer0();
    initTimer1();
    sei(); // Enable global interrupts

    // Main loop
    while (!((distance <= 0) || ((distance <= 3) && (setSpeed <= 5)))) {
        showParameter();
    }

    TCCR1B &= ~TIMER1_PRESCALER; // Stop timer 1
    TCCR0B |= TIMER0_PRESCALER; // Stop timer 0

    // Clear display and display output logs
    wipeDisplay();
    for (int i = 0; i < logIndex; i++) {
        printf("%d: value %d, value: %d, value %d, value %d\n", 
                i, output[i]->distance, output[i]->setSpeed, output[i]->fuel, output[i]->explosion);
        free(output[i]);
    }

   
    if (setSpeed <= 5) {
        playBuzz(NOTE_C5, 90);
        endBuzz();
        _delay_ms(10);
        playBuzz(NOTE_E5, 90);
        endBuzz();
        _delay_ms(10);
        playBuzz(NOTE_G5, 90);
        endBuzz();
        while (1) {
            writeString("Success");
        }
    } else {
        playBuzz(NOTE_G5, 90);
        endBuzz();
        _delay_ms(10);
        playBuzz(NOTE_E5, 90);
        endBuzz();
        _delay_ms(10);
        playBuzz(NOTE_C5, 90);
        endBuzz();
        while (1) {
            writeString("Game Over - Failure");
        }
    }
}
