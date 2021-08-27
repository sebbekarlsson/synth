#ifndef ROB_LIST_H
#define ROB_LIST_H
#include <stdint.h>

typedef struct {
  uint32_t size;
  void** items;
  unsigned int item_size;
} list_T;

list_T* list_init(unsigned int item_size);

void list_push(list_T* list, void* item);

void* list_at(list_T* list, uint32_t index);

void list_shift_left(list_T* list, int index);

void list_remove(list_T* list, void* element, void (*free_method)(void* item));

void list_empty(list_T* list);

void* list_pop(list_T* list);

#endif
