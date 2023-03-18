#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

void delete_from(WP* list, WP* victim) {
  WP* ptr = list;
  if(list == victim) {
    //链表头就是要删除的对象
    list = list->next;
    ptr->next = NULL;
    return ;
  }
  while(ptr->next != victim) ptr = ptr->next;
  ptr->next = victim->next;
  victim->next = NULL;
}


void add_tail(WP* list, WP* victim) {
  WP* ptr = list;
  if(list == NULL) {
    list = victim;
    Log("First insert");
    Log("head %d", (uint32_t)head);
  }
  else {
    while(ptr->next) ptr = ptr->next;
    ptr->next = victim;
  } 
}

void add_to(WP* list, WP* victim) {
  add_tail(list, victim);
}

WP* new_wp() {
  WP* victim = free_;//选择从头删除，free_以fifo的形式维护
  if(victim) {//free不为空，有空闲监视点
    add_to(head,victim);
    delete_from(free_,victim);
    return victim;
  } else {//无空闲监视点
    assert(0);
  }
  return victim;
}

void free_wp(int N) {
  WP* ptr = head;
  while(ptr->NO != N) {
    ptr = ptr->next;
  }
  if(!ptr) assert(0);
  else {
    add_to(free_, ptr);
    delete_from(head, ptr);
  } 
}

void print_wps() {
  WP *temp = head;
  if (temp == NULL){
    printf("No watchpoint\n");
  }
  while (temp != NULL){
    printf("Watch point NO: %d ,Expr: %s ,Expr_val: %d .\n", temp->NO, temp->expr, temp->expr_val);
    temp = temp->next;
  }
}

bool check_wps() {
  WP* wp = head;
  bool is_changed = false;
  while(wp) {
    bool success = true;
    uint32_t new_val = expr(wp->expr, &success);
    if(new_val != wp->expr_val) {
      Log("Watchpoint NO.%d find an old val:%d is update to %d.", wp->NO, wp->expr_val, new_val);
      wp->expr_val = new_val;
      is_changed = true;
    } 
    wp = wp->next;
  }
  return is_changed;
}

/* TODO: Implement the functionality of watchpoint */


