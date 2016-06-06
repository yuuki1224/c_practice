
#include <stdio.h>
#include <stdlib.h>

/*
 FILE fopen()
 fclose()
 fgets()
*/
static char *filePath = "./files/hello_world";

void showFileContent(void) {
  FILE *fp;
  fp = fopen(filePath, "r");
  if (fp == NULL) {
    printf("fopen failed.\n");
    exit(EXIT_FAILURE);
  }
  
  printf("==================\n");
  char s[256];
  while (fgets(s, 256, fp) != NULL) {
    printf("%s", s);
  }
  printf("\n");
  printf("==================\n");
  
  fclose(fp);
}

void writeToFile(char *s) {
  FILE *fp;
  fp = fopen(filePath, "a");
  if (fp == NULL) {
    printf("fopen failed.\n");
    exit(EXIT_FAILURE);
  }
  
  fputs(s, fp);
  fclose(fp);
}

int main(void) {
  char s[256];
  
  while (1) {
    printf("Show: s, Write: w\n");
    scanf("%s", s);
    
    switch (s[0]) {
      case 's':
        showFileContent();
        break;
      case 'w': {
        char t[256];
        printf("What do you wanna write?\n");
        scanf("%s", t);
        writeToFile(t);
        break;
      }
      default:
        break;
    }
  }
}