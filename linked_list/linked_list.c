
/* Linked list を操作する関数 */
#include "linked_list.h"

// List * make_list(void)
static Cell * make_cell(int val, Cell *next_cell) {
  Cell *cell = malloc(sizeof(Cell));
  if (cell != NULL) {
    cell->content = val;
    cell->next = next_cell;
  }
  return cell;
}

List * make_list(void) {
  List *list = malloc(sizeof(List));
  if (list != NULL) {
    Cell *cell = make_cell(0, NULL);
    list->first = cell;
    if (list->first == NULL) {
      free(list);
      return NULL;
    }
  }
  return list;
}

void delete_cell(Cell *cell) {
  while (cell != NULL) {
    Cell *tmp = cell->next;
    free(cell);
    cell = tmp;
  }
}

// void delete_list(List *list)
void delete_list(List *list) {
  delete_cell(list->first);
  free(list);
}

// int nth(Cell *cell, int n)
Cell * nth_cell(Cell *cell, int n) {
  for (int i = -1; cell != NULL; i++, cell = cell->next) {
    if(i == n) break;
  }
  return cell;
}

int nth(List *list, int n, bool *err) {
  Cell *cell = nth_cell(list->first, n);
  if (cell == NULL) {
    *err = false;
    return 0;
  }
  *err = true;
  return cell->content;
}

// bool insert_nth(List *ls, int n, int val)
bool insert_nth(List *list, int n, int val) {
  Cell *cell = nth_cell(list->first, n-1);
  if (cell == NULL) return false;
  cell->next = make_cell(val, cell->next);
  return true;
}

// bool push(List *ls, int val)
bool push(List *list, int val) {
  return insert_nth(list, 0, val);
}

// bool delete_nth(List *ls, int n)
bool delete_nth(List *list, int n) {
  Cell *cell = nth_cell(list->first, n-1);
  if (cell == NULL || cell->next == NULL) return false;
  Cell *tmp = cell->next;
  cell->next = cell->next->next;
  free(tmp);
  return true;
}

// int pop(List *ls, bool *error)
int pop(List *list, bool *err) {
  int content = nth(list, 0, err);
  if (*err) delete_nth(list, 0);
  return content;
}

// bool empty_list(List *ls)
bool empty_list(List *list) {
  return list->first->next == NULL;
}

// void print_list(List *list)
void print_list(List *list) {
  printf("Linked list: ");
  for (Cell *cell = list->first->next; cell != NULL; cell = cell->next) {
    printf("(%d)", cell->content);
    if(cell->next != NULL) printf(" -> ");
  }
  printf("\n");
}
