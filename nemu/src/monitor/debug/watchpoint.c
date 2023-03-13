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
WP* new_wp() {
  WP* victim = free_;//选择从头删除，free_以fifo的形式维护
  if(victim) {//free不为空，有空闲监视点
    add_to(head,victim);
    delete_from(free_,victim);
  } else {//无空闲监视点
    assert(0);
  }
}

void free_wp(WP* wp) {
  WP* ptr = head, ptr2 = free_;
  add_to(free_, wp);
  delete_from(head,wp);
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

void add_to(WP* list, WP* victim) {
  add_tail(list, victim);
}

void add_tail(WP* list, WP* victim) {
  WP* ptr = list;
  if(!list) list = victim;
  else {
    while(ptr->next) ptr = ptr->next;
    ptr->next = victim;
  } 
}


/* TODO: Implement the functionality of watchpoint */


