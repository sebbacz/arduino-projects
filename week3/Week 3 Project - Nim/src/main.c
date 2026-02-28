#include <avr/io.h>
#include <DISPLAY_LIB.h>
#include <util/delay.h>
#include <stdio.h>
#include <usart.h>

#include <stdlib.h>

#define FALSE 0
#define START_NUMBER 10
#define BUTTON3 PC3
#define BUTTON2 PC2
#define BUTTON1 PC1

#define MAX_MOVES 10  // maximum moves
#define maximum 3

#define TRUE 1
#define minimum 1



typedef struct {
    int leftover_mach;
} OUTPUT;

typedef struct {
    int matchesLeft[MAX_MOVES];
    int next_step;
} GAME;

int RDM(int min, int max) {
    //simple generator
    return rand() % (max - min + 1) + min;
}

void stats(int digit, char player, int matchesLeft) {
    int displayValue = digit * 1000 + matchesLeft;
    writeNumber(displayValue);
    
}

void stats_display(int digit, char player, int matchesLeft, int delayMs) {
    stats(digit, player, matchesLeft);
    _delay_ms(delayMs);
}

void game_resut(GAME* game, OUTPUT ps, OUTPUT computerStats, int currentMove) {
    printf("player matches: %d\n", ps.leftover_mach);
    printf("computer matches: %d\n", computerStats.leftover_mach);
  
}

void next_move(char player) {
    int displayNumber = (player == 'P') ? 1111 : 2222;
    for (int i = 0; i < 3; i++) {
        writeNumberAndWait(displayNumber, 400);
        _delay_ms(400);
    }
}

void waitForButton(int button, int* digit, char player, int matchesLeft) {
    int pressed = FALSE;
    while (!pressed) {
        stats(*digit, player, matchesLeft);
        if (bit_is_clear(PINC, button)) {
            pressed = TRUE;
            stats_display(*digit, player, matchesLeft, 200);
        }
    }
}

int mix(int value, int min, int max) {
    if (value > max) return max;
    if (value < min) return min;
    return value;
}

void startgame() {
    // Configure button pins as inputs
    DDRC &= ~(1 << BUTTON1);
    DDRC &= ~(1 << BUTTON2);
    DDRC &= ~(1 << BUTTON3);

    // Enable pull-up resistors on buttons
    PORTC |= (1 << BUTTON1);
    PORTC |= (1 << BUTTON2);
    PORTC |= (1 << BUTTON3);

    // Initialize serial communication and display
    initUSART();
    initDisplay();
}

int main() {
    startgame();

    int matchesLeft = START_NUMBER;
    char player = 'P';
    int digit = 2;

    GAME* game = malloc(sizeof(GAME));
    game->next_step = 0;

    OUTPUT ps = {0};
    OUTPUT computerStats = {0};

    while (matchesLeft > 1) {
        if (player == 'P') {
            next_move(player);

            int done = FALSE;
            while (!done) {
                stats(digit, player, matchesLeft);

                if (bit_is_clear(PINC, BUTTON1)) {
                    digit -= 1;
                    digit = mix(digit, minimum, maximum);
                    stats_display(digit, player, matchesLeft, 200);
                }

                if (bit_is_clear(PINC, BUTTON3)) {
                    digit += 1;
                    digit = mix(digit, minimum, maximum);
                    stats_display(digit, player, matchesLeft, 200);
                }

                if (bit_is_clear(PINC, BUTTON2)) {
                    done = TRUE;
                }
            }

            ps.leftover_mach += digit;
            matchesLeft -= digit;
            player = 'C';

            if (game->next_step < MAX_MOVES) {
                game->matchesLeft[game->next_step] = matchesLeft;
                game->next_step++;
            }
        } else if (player == 'C') {
            next_move(player);

            waitForButton(BUTTON2, &digit, player, matchesLeft);

            digit = (matchesLeft - 1) % (maximum + 1);
            if (digit == 0) {
                digit = RDM(minimum, maximum);
            }

            waitForButton(BUTTON2, &digit, player, matchesLeft);

            computerStats.leftover_mach += digit;
            matchesLeft -= digit;
            player = 'P';

            if (game->next_step < MAX_MOVES) {
                game->matchesLeft[game->next_step] = matchesLeft;
                game->next_step++;
            }
        }

        stats(digit, player, matchesLeft);
    }

    printf("Game over\n");
    game_resut(game, ps, computerStats, game->next_step);
    next_move(player);

    free(game);
    return 0;
}
