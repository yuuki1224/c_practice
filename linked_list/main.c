/*
  連結リスト データを一方向につなげたデータ構造
   o -> o -> o -> o
   参考: http://www.geocities.jp/m_hiroi/linux/clang12.html
 */
 #include <stdio.h>
 #include "linked_list.h"

int main(void) {
  List *list = make_list();
  
  push(list, 1);
  push(list, 2);
  push(list, 3);
  
  print_list(list);
  
  bool err;
  printf("%d\n", pop(list, &err));
  
  delete_list(list);
}
