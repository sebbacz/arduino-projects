
#include <stdio.h>

#define MAX 5 //size of array
#define SEVENFOLD(x) ((x) * 7) //multiplies x by 7

// print array
void printArray(int *arr, int size) {
    printf("Content of array:\n");
    for (int i = 0; i < size; i++) {
        printf("  Address: %p has value: %d\n", (void*)&arr[i], arr[i]);
    }
}

// fill aray
void makeArray(int *arr, int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = SEVENFOLD(i);
    }
}

int main() {
    int array[MAX] = {0};

    // print array
    printArray(array, MAX);

    // fill array
    makeArray(array, MAX);
    printArray(array, MAX);

    return 0;
}
