#ifndef ROB_LIST_H
#define ROB_LIST_H
#include <pthread.h>
#include <stdint.h>

typedef struct {
  uint32_t size;
  void **items;
  unsigned int item_size;
  pthread_mutex_t lock;
} list_T;

list_T *list_init(unsigned int item_size);

void list_push(list_T *list, void *item);

void *list_at(list_T *list, uint32_t index);

void list_shift_left(list_T *list, int index);

void list_remove(list_T *list, void *element, void (*free_method)(void *item));

void list_empty(list_T *list);

void *list_pop(list_T *list);

#define LIST_ITER(list, C, NAME, WHAT)                                         \
  uint32_t lsize = list->size;                                                 \
  if (list && list->items && lsize) {                                          \
    for (uint32_t C = 0; C < lsize; C++) {                                     \
      void *NAME = (void *)list_at(list, C);                                   \
      WHAT;                                                                    \
    }                                                                          \
  }

#endif
