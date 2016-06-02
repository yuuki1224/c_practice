
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define HASH_SIZE 40
#define MAX_KW_LEN 256
#define NUM_KW 23

typedef struct list {
  char keyword[MAX_KW_LEN];
  struct list *next;
} List;

static struct list * hashTable[MAX_KW_LEN];

static char keywords[NUM_KW][MAX_KW_LEN] = {
  "auto", "break", "double", "enum", "char", "continue", "extern", 
  "float", "for", "int", "long", "register", "short", "signed", "static",
  "struct", "typedef", "union", "unsigned", "return", "void", "volatile", "while"
};

// Hash 関数
int hashFunction(char *key) {
  int hashValue = 0;
  while (*key != '\0')
    hashValue += *key++;
  return (hashValue % HASH_SIZE);
}

bool findKeyword(char *keyword) {
  List *list;
  for (list = hashTable[hashFunction(keyword)]; list != NULL; list = list->next) {
    if (!strcmp(keyword, (*list).keyword)) return true;
  }
  return false;
}

// HashTableの初期化
void initHashTable() {
  for (int i=0; i < NUM_KW; i++) {
    if (!findKeyword(keywords[i])) {
      List *list = malloc(sizeof(List));
      if(list == NULL) return;
      
      strcpy((*list).keyword, keywords[i]);
      int hashValue = hashFunction(keywords[i]);
      
      if (hashTable[hashValue] == NULL) {
        hashTable[hashValue] = list;
        list->next = NULL;
      } else {
        List *existingList = hashTable[hashValue];
        while (existingList->next != NULL)
          existingList = existingList->next;
        existingList->next = list;
        list->next = NULL;
      }
    }
  }
}

void listKeyword(void) {
  for (int i=0; i < HASH_SIZE; i++) {
    for (List *list = hashTable[i]; list != NULL; list = list->next) {
      printf("Keyword: %s, Hash: %d\n", (*list).keyword, hashFunction((*list).keyword));
    }
  }
}

void freeHashTable(void) {
  for (int i=0; i < HASH_SIZE; i++) {
    for (List *list = hashTable[i]; list != NULL; ) {
      List *tmp = list->next;
      free(list);
      list = tmp;
    }
  }
}

int main(void) {
  initHashTable();
  listKeyword();
  
  char keyword[MAX_KW_LEN];
  
  printf("Enter Keyword:\n");
  
  gets(keyword);
  
  if (findKeyword(keyword)) {
    printf("%s is registered keyword: Hash Value => %d\n", keyword, hashFunction(keyword));
  } else {
    printf("%s is not registered keyword.\n", keyword);
  }
  
  freeHashTable();
}