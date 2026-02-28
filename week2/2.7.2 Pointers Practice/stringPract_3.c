#include <stdio.h>
#include <string.h>

#define MAX 7

void printFirstLetters(char names[MAX][10]) {
    printf("First letters: ");
    for (int i = 0; i < MAX; i++) {
        printf("%c", names[i][0]);
    }
    printf("\n");
}

void printLastLetters(char names[MAX][10]) {
    printf("Last letters: ");
    for (int i = 0; i < MAX; i++) {
        int len = strlen(names[i]);
        printf("%c", names[i][len - 1]);
    }
    printf("\n");
}

// Function to search for the shortest name in the array
char* searchShortestName(char names[MAX][10]) {
    char *smallest = names[0]; // 
    for (int i = 1; i < MAX; i++) {
        if (strcmp(names[i], smallest) < 0) { // compare
            smallest = names[i]; // update smallest
        }
    }
    return smallest; // return the smallest name
}

int main() {
    char names[MAX][10] = {"eee", "wdfwdef", "223r23r", "23r2", "23r", "fws", "f2ef"};

    // first letters
    printFirstLetters(names);

    //  last letters
    printLastLetters(names);

    // search smaleltst one
    char *smallest = searchShortestName(names);
    printf("Shortest = %s\n", smallest);

    return 0;
}