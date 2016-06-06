
#include <stdio.h>

#define NUM_DATA 10
int numbers[NUM_DATA] = {3, 5, 2, 7, 4, 6, 11, 20, 100, 12};

void sort(int numbers[], int i, int j) {
  if (numbers[i] > numbers[j]) {
    int tmp = numbers[j];
    numbers[j] = numbers[i];
    numbers[i] = tmp;
  }
}

void bubbleSort(int numbers[]) {
  for (int i=NUM_DATA; 1 < i; i--) {
    for (int j=1; j < i; j++) {
      sort(numbers, j-1, j);
    }
  }
}

void printNumbers(int numbers[]) {
  printf("==========================================\n");
  printf("[");
  for (int i=0; i < NUM_DATA; i++) {
    printf("%d", numbers[i]);
    if (i != NUM_DATA-1) printf(", ");
  }
  printf("]\n");
  printf("==========================================\n");
}

int main(void) {
  printNumbers(numbers);
  
  bubbleSort(numbers);
  
  printNumbers(numbers);
}