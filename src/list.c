#include <list.h>
#include <stdio.h>
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
  if (index >= list->size || !list->items) {
    return 0;
  }
  void *item = list->items[index];
  return item;
}

void list_shift_left(list_T *list, int index) {
  for (int i = index; i < list->size - 1; i++)
    list->items[i] = list->items[i + 1];
}

void list_remove(list_T *list, void *element, void (*free_method)(void *item)) {
  if (list->size == 0) {
    return;
  }

  int index = -1;

  for (int i = 0; i < list->size; i++) {
    if (list->items[i] == element) {
      index = i;
      break;
    }
  }

  if (index == -1) {
    return;
  }

  if (free_method != (void *)0)
    free_method(list->items[index]);

  list_shift_left(list, index); /* First shift the elements, then reallocate */
  void *tmp = realloc(list->items, (list->size - 1) * list->item_size);
  if (tmp == NULL && list->size > 1) {
    /* No memory available */
    exit(EXIT_FAILURE);
  }
  list->size = list->size - 1;
  list->items = tmp;
}

void list_empty(list_T *list) {
  if (!list->size || !list->items) {

    return;
  }

  free(list->items);
  list->items = 0;
  list->size = 0;
}

void *list_pop(list_T *list) {
  if (!list->size || list->items == 0) {
    return 0;
  }
  void *item = list->items[list->size - 1];

  if (!item) {
    return 0;
  }

  list_remove(list, item, 0);

  return item;
}
