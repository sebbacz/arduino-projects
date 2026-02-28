#define __DELAY_BACKWARD_COMPATIABLE__  // delay funcs
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#define DEFUSE_CODE_SIZE  3
#define SENSOR_THRESHOLD  200  //adc

typedef uint8_t boolean;
typedef uint8_t byte;
typedef uint16_t word;

// global variables
boolean sensor_mode = 0;  // Flag to enable/disable sensor mode 0-off 1-on
boolean bomb_triggered_by_sensor = 0; // Flag to check if bomb was triggered by sensor
volatile byte number_display = 0;
byte codeindex = 0;
volatile word adc_val = 0;
volatile byte sequence = 0;
byte status = 0;
volatile byte buffer[] = {0, 0, 0, 0};
byte set_time = 0;
byte stage = 0;
byte number = 0;
boolean button1_active = 0;
boolean button2_activate = 0;
boolean button3_activate = 0;
char secret_code[DEFUSE_CODE_SIZE + 1] = {0};
int16_t time_to_explode = 0;



boolean buttons[3] = {1, 1, 1};
const char DEGUSE_CODE_CONSTANT[DEFUSE_CODE_SIZE + 1] = {'1', '2', '3', 0};

// Libraries
#include "ADC_LIB.h"
#include "BTN_LIB.h"
#include "BUZZER_LIB.h"
#include "DISPLAY_LIB.h"
#include "LED_LIB.h"
#include "usart.h"

// Function prototypes
void timer1(void);
void timer2(void);
boolean get_secret_code(void);
void update_buffer(volatile byte* buffer);
void run(void);
void functionality(void);

void run(void) {
    cli(); // disable interrupts
    configure_leds(); 
    initUSART();
    initDisplay();
    initADC();
    enableButton(1);
    enableButton(2);
    enableButton(3);
    configure_buzzer();
    timer2();
    startADC();
    sei(); // enable global interrupts

    // Check if sensor mode should be enabled 
    if (buttonPushed(1)) {
        sensor_mode = 1;
        printf("Sensor mode enabled\n");
    }
}

// ADC interrupt service routine
ISR(ADC_vect) { 
    adc_val = ADC;  // Read the ADC value

    // Check if sensor mode is enabled and bomb should be triggered
    if (sensor_mode && adc_val < SENSOR_THRESHOLD) {
        bomb_triggered_by_sensor = 1;  // Set flag that bomb is triggered by sensor
    }
}

// Button interrupt service routine
ISR(PCINT1_vect) { 
    uint8_t button_state = (PINC & 0x0E) >> 1;
    buttons[0] = (button_state & 0x01) ? 1 : 0;
    buttons[1] = (button_state & 0x02) ? 1 : 0;
    buttons[2] = (button_state & 0x04) ? 1 : 0;
    save_btn_state(buttons);
}

// Buzzer control interrupt service routine
ISR(TIMER0_COMPA_vect) {
    PORTD ^= _BV(PD3);
}

// Timer 1 interrupt service routine
ISR(TIMER1_COMPA_vect) { 
    sequence++;
    if ((set_time - sequence) > 5) {
        // 100 Hz buzzer beep
        PORTD &= ~_BV(PD3);
        _delay_ms(5);
        // Turn off the buzzer
        PORTD |= _BV(PD3);
    }
    status = 1;
}

// Timer 2 interrupt service routine
ISR(TIMER2_COMPA_vect) {
    writeNumberToSegment(number_display, buffer[number_display]);
    if (number_display++ > 3) {
        number_display = 0; 
        startADC();
    }
}

void update_buffer(volatile byte* buffer) {
    buffer[0] = number / 1000;
    buffer[1] = (number / 100) % 10;
    buffer[2] = (number / 10) % 10;
    buffer[3] = number % 10;  
}

void timer1(void) {
    TCCR1A = 0;  
    TCCR1B = _BV(WGM12);   // CTC mode
    OCR1A  = 15625;        // Set compare value for 1s interval
    TCCR1B |= _BV(CS12) | _BV(CS10);  // Prescaler 1024
    TIMSK1 = _BV(OCIE1A);  // Enable Timer 1 interrupt
}

void timer2(void) {
    TCCR2A = _BV(WGM21);   // CTC mode
    OCR2A  = 78;           // Set compare value for 5ms interval
    GTCCR = _BV(PSRASY);   // Reset prescaler
    TCCR2B = _BV(CS22) | _BV(CS21) | _BV(CS20);  // Prescaler 1024
    TIMSK2 = _BV(OCIE2A);  // Enable Timer 2 interrupt
    PCICR = _BV(PCIE1);   
    PCIFR = _BV(PCIF1);    // Clear pin change interrupt flag
}

boolean get_secret_code(void) {
    if (buttonPushed(1) && !button1_active) {
        secret_code[codeindex++] = '1';
        button1_active = 1;
    }
    if (!buttonPushed(1) && button1_active) {
        button1_active = 0;
    }
    if (buttonPushed(2) && !button2_activate) {
        secret_code[codeindex++] = '2';
        button2_activate = 1;
    }
    if (!buttonPushed(2) && button2_activate) {
        button2_activate = 0;
    }
    if (buttonPushed(3) && !button3_activate) {
        secret_code[codeindex++] = '3';
        button3_activate = 1;
    }
    if (!buttonPushed(3) && button3_activate) {
        button3_activate = 0;
    }
    // Check if the secret code length is met
    if (codeindex >= DEFUSE_CODE_SIZE) {
        // Check if the secret code matches
        if (!strcmp(DEGUSE_CODE_CONSTANT, secret_code)) {
            return 1;    
        }
        codeindex = 0;
    }
    return 0;
}

void clear_secret_code(void) {
    for (byte i = 0; i < sizeof(secret_code); i++) {
        secret_code[i] = 0;  // Manually clear the secret code
    }
}

void functionality(void) {
    switch (stage) {
        case 0:
            // Configuration of the countdown timer
            do {
                number = mapADC(adc_val);
                update_buffer(buffer);
                _delay_ms(20);
            } while (!buttonPushed(2)); // button S2
            set_time = number;
            _delay_ms(300);
            stage = 1;
            timer1(); 
            clear_secret_code();
            break;

        case 1:
            // Check if the bomb should detonate due to proximity
            if (bomb_triggered_by_sensor) {
                TCCR1B = 0;  // Stop Timer 1
                stage = 2;   // Go to explode stage
                bomb_triggered_by_sensor = 0;
                printf("BOOOM! Bomb triggered by proximity sensor\n");
                sequence = set_time;
                timer1();
            }

            // Secret code input and time countdown
            if (get_secret_code()) {
                TCCR1B = 0;  // Stop Timer 1
                stage = 3;
                codeindex = 0;
                all_led_off();
                clear_secret_code();
            } else {
                if (status) {
                    status = 0;
                    time_to_explode = set_time - sequence;
                    if (time_to_explode >= 0) {
                        number = time_to_explode;
                        update_buffer(buffer);
                        printf("time left: %d\n", time_to_explode);
                        if (time_to_explode >= 0 && time_to_explode <= 5) {
                            // Handle last 5 seconds
                            switch (number) {
                                case 5: set_beep_freq(100); all_led_off(); break;
                                case 4: set_beep_freq(500); all_led_off(); break;
                                case 3: set_beep_freq(1000); plan1(); break;
                                case 2: set_beep_freq(2000); plan2(); break;
                                case 1: set_beep_freq(4000); plan3(); break;
                                case 0: set_beep_freq(50); all_led_on(); break;
                            }
                        }
                    } else {
                        if (sequence >= set_time) {
                            TCCR1B = 0;
                            stage = 2;
                            codeindex = 0;
                            set_beep_freq(50);
                            clear_secret_code();
                            printf("BOOOM\n");
                            sequence = set_time;
                            timer1();
                        }
                    }
                }
            }
            break;

        case 2:
            // Explode stage
            time_to_explode = set_time - sequence;
            switch (time_to_explode) {
                case 0: buffer[0] = 10; buffer[1] = 11; buffer[2] = 10; buffer[3] = 10; break;
                case -1: buffer[0] = 10; buffer[1] = 12; buffer[2] = 13; buffer[3] = 10; break;
                case -2: buffer[0] = 10; buffer[1] = 14; buffer[2] = 15; buffer[3] = 10; break;
                case -3: buffer[0] = 16; buffer[1] = 14; buffer[2] = 15; buffer[3] = 16; break;
                case -4: buffer[0] = 17; buffer[1] = 17; buffer[2] = 17; buffer[3] = 17; break;
                case -5:
                    TCCR1B = 0;  // Stop Timer 1
                    stage = 3;
                    set_beep_freq(0);
                    break;
            }
            _delay_ms(100);
            break;

        case 3:
            // Final stage after explosion or defusal
            break;
    }
}



// Main function
int main(void) {
    run();

    while (1) {
        functionality();
    }
    return 0;
}
