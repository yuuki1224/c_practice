
#include <stdio.h>

#define NUM_DATA 10
int numbers[NUM_DATA] = {1, 10, 34, 55, 100, 28, 90, 31, 2, 22};

void swap(int numbers[], int i, int j) {
  int tmp = numbers[j];
  numbers[i] = numbers[j];
  numbers[j] = tmp;
}

int midNum(int min, int max) {
  return (min + max) / 2;
}

void quickSort(int numbers[], int i, int j) {
}

void printNumbers(void) {
  printf("========================================\n");
  printf("[");
  for (int i=0; i<NUM_DATA; i++) {
    printf("%d", numbers[i]);
    if (i != NUM_DATA-1) printf(", ");
  }
  printf("]\n");
  printf("========================================\n");
}

int main(void) {
  int i = 0;
  int j = 4;
  printf("%d\n", midNum(i, j));
}