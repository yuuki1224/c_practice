
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define M 2

typedef struct page {
  int n; /* 要素数 */
  int key[M * 2];
  struct page *branch[M * 2 + 1];
} Page;

int _key;
Page * _root = NULL;
Page * _newPage = NULL;

bool _done = false;

bool _debug = false;

int _depth = 1;

Page * createPage(void) {
  Page * page = malloc(sizeof(Page));
  if (page == NULL) return NULL;
  
  page->n = 0;
  for (int i=0; i < M * 2; i++)
    page->branch[i] = NULL;
    
  return page;
}

void search(Page *page, int key) {
  if (page == NULL) return;
  
  int i;
  for (i=0; i < page->n && key >= page->key[i]; i++) {
    if (page->key[i] == key) {
      printf("%d ありました!\n", key);
    }
  }
  
  if (page->branch[i] != NULL) {
    search(page->branch[i], key);
  }
}

void insertKey(Page *page, int k) {
  if (page->n < k) return;
  
  for (int i = page->n; i > k; i--) {
    page->key[i] = page->key[i-1];
    page->branch[i+1] = page->branch[i];
  }
  
  page->key[k] = _key;
  page->n++;
}

void splitPage(Page *page) {
  int m = (page->n % 2) == 0 ? page->n / 2 : page->n / 2 + 1;
  Page *newPage = createPage();
  
  for (int i=m; i < page->n; i++) {
    newPage->key[i - m] = page->key[i];
    newPage->branch[i - m + 1] = page->branch[i + 1];
    
    newPage->branch[i - m + 1] = NULL;
  }
  
  newPage->n = page->n - m;
  printf("New: %d\n", newPage->n);
  page->n = page->n - newPage->n;
  
  _key = page->key[page->n-1];
  newPage->branch[0] = page->branch[page->n];
  
  page->branch[page->n] = NULL;
  page->n--;
  printf("Page: %d\n", page->n);
  _newPage = newPage;
}

void insertSub(Page *page) {
  int i;
  for (i=0; i < page->n && _key >= page->key[i] ; i++) {
    if (page->key[i] == _key) {
      printf("%d は既に存在します.\n", _key);
      return;
    }
  }
  
  if (_debug) {
    printf("%d\n", i);
  }
  
  Page *foundBranch = page->branch[i];
  if (foundBranch != NULL) printf("found branch: %d\n", foundBranch->n);
  
  if (foundBranch != NULL) insertSub(foundBranch);
  
  if (!_done) {
    if (page->n + 1 == M * 2) {
      insertKey(page, i);
      splitPage(page);
      _done = false;
    } else {
      insertKey(page, i);
      if (_newPage != NULL) {
        page->branch[i+1] = _newPage;
      }
      _done = true;
    }
  }
}

void insert(Page *page) {
  _done = false;
  insertSub(_root);
  
  if (_done) {
    return;
  } else {
    Page *newPage = createPage();
    newPage->n = 1;
    newPage->key[0] = _key;
    newPage->branch[0] = _root;
    newPage->branch[1] = _newPage;
    _root = newPage;
    
    _newPage = NULL;
  }
}

void printPage(Page *page) {
  printf("====================================\n[");
  for (int i=0; i < page->n; i++) {
    printf("%d", page->key[i]);
    if(i + 1 < page->n) printf(" ,");
  }
  printf("] Num: %d\n====================================\n", page->n);
}

void printBranches(Page *page) {
  for (int i=0; i <= page->n; i++) {
    Page *branch = page->branch[i];
    
    printf("[");
    for(int j=0; j < branch->n; j++) {
      printf("%d", branch->key[j]);
      if (j != branch->n-1) printf(" ,");
    }
    printf("]  ");
    // printf("Num: %d  ", page->branch[i]->n);
  }
  printf("\n");
}

void hasBranches(Page *page) {
  for (int i=0; i<=page->n; i++) {
    Page *branch = page->branch[i];
    if (branch != NULL) {
      printf("[%d] Has branch\n", i);
    }
  }
}

void freePage(Page *page) {
  free(page);
}

int main(void) {
  _root = createPage();
  
  _key = 1;
  insert(_root);
  
  _key = 2;
  insert(_root);
  
  _key = 3;
  insert(_root);
  
  // _key = 10;
  // insert(_root);
  // 
  // _key = 11;
  // insert(_root);
  // 
  // _key = 4;
  // insert(_root);
  // 
  // _key = 5;
  // insert(_root);
  // 
  // _key = 20;
  // insert(_root);
  
  // _key = 6;
  // insert(_root);
  
  printPage(_root);
  // printBranches(_root);
  
  // search(_root, 3);
  // search(_root, 20);
  // search(_root, 4);
  // search(_root, 10);
  // printf("%d\n", _root->branch[1]->n);
  // 
  // hasBranches(_root->branch[0]);
  // hasBranches(_root->branch[1]);
  // hasBranches(_root->branch[2]);
  // hasBranches(_root->branch[3]);
}
