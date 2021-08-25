#include <list.h>
#include <stdlib.h>

list_T *list_init(unsigned int item_size) {
  list_T *list = (list_T *)calloc(1, sizeof(list_T));
  list->item_size = item_size;
  list->items = 0;
  return list;
}

void list_push(list_T *list, void *item) {
  list->size += 1;
  list->items = realloc(list->items, list->size * list->item_size);
  list->items[list->size - 1] = item;
}

void *list_at(list_T *list, uint32_t index) {
  if (index >= list->size || !list->items)
    return 0;
  return list->items[index];
}
