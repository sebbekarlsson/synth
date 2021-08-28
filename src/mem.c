#include <mem.h>
#include <stdio.h>
#include <stdlib.h>

void *scalloc(uint32_t length, unsigned int siz) {
  void *memory = calloc(length, siz);
  if (memory == 0) {
    printf("Could not allocate a memory size of %d * %d", length, siz);
    exit(1);
  }

  return memory;
}
