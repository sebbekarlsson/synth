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

#endif
