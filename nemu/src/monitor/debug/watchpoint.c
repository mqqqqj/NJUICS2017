#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

void init_wp_pool()
{
  int i;
  for (i = 0; i < NR_WP; i++)
  {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

WP *new_wp()
{
  WP *victim = free_; // 选择从头删除，free_以fifo的形式维护
  if (victim)
  { // free不为空，有空闲监视点
    // 判断head是否为null，即判断是否为第一次插入
    if (!head)
    {
      // first insert
      head = victim;
      free_ = free_->next;
      victim->next = NULL;
    }
    else
    {
      WP *ptr = head;
      while (ptr->next)
        ptr = ptr->next;
      ptr->next = victim;
      free_ = free_->next;
      victim->next = NULL;
    }
    return victim;
  }
  else
  { // 无空闲监视点
    assert(0);
  }
  return victim;
}

void free_wp(int N)
{
  WP *victim = head;
  while (victim && victim->NO != N)
  {
    victim = victim->next;
  }
  if (!victim)
    assert(0);
  else
  {
    memset(victim->expr, 0, 100 * sizeof(char));
    // add victim to the tail of free_
    if (!free_)
    {
      // free_ is empty
      free_ = victim;
      // delete victim from head
      if (victim == head)
      {
        head = head->next;
        victim->next = NULL;
      }
      else
      {
        WP *p_vic = head;
        while (p_vic->next->NO != N)
        {
          p_vic = p_vic->next;
        }
        p_vic->next = victim->next;
        victim->next = NULL;
      }
    }
    else
    {
      // add victim to the tail of free_
      WP *l_free = free_;
      while (l_free->next)
        l_free = l_free->next;
      l_free->next = victim;
      // delete victim from head
      // 1. is head have one node?
      // 2. head is the node to be deleted?
      if (head->next == NULL)
      {
        // head must be the victim, delete head
        head = NULL;
      }
      else
      {
        if (victim == head)
        {
          head = head->next;
          victim->next = NULL;
        }
        else
        {
          WP *p_vic = head;
          while (p_vic->next->NO != N)
          {
            p_vic = p_vic->next;
          }
          p_vic->next = victim->next;
          victim->next = NULL;
        }
      }
    }
  }
}

void print_wps()
{
  WP *temp = head;
  if (temp == NULL)
  {
    printf("No watchpoint\n");
  }
  while (temp != NULL)
  {
    printf("Watch point NO: %d ,Expr: %s ,Expr_val: %d .\n", temp->NO, temp->expr, temp->expr_val);
    temp = temp->next;
  }
}

bool check_wps()
{
  WP *wp = head;
  bool is_changed = false;
  while (wp)
  {
    bool success = true;
    uint32_t new_val = expr(wp->expr, &success);
    if (new_val != wp->expr_val)
    {
      Log("Watchpoint NO.%d find an old val:%d is update to %d.", wp->NO, wp->expr_val, new_val);
      wp->expr_val = new_val;
      is_changed = true;
    }
    wp = wp->next;
  }
  return is_changed;
}

/* TODO: Implement the functionality of watchpoint */
