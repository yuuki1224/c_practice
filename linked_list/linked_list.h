
#ifndef _LINKED_LIST_H
#define _LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct cell {
  int content;
  struct cell *next;
} Cell;

typedef struct {
  Cell *first;
} List;

// functions

// Cell * make_cell(int val, Cell *next_cell);
List * make_list(void);
void delete_cell(Cell *cell);
void delete_list(List *list);
Cell * nth_cell(Cell *cell, int n);
int nth(List *list, int n, bool *err);
bool insert_nth(List *list, int n, int val);
bool push(List *list, int val);
bool delete_nth(List *list, int n);
int pop(List *list, bool *err);
bool empty_list(List *list);
void print_list(List *list);

#endif