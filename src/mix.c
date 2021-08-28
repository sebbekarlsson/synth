#include <mix.h>
#include <stdio.h>

int mix_buffers(float **data1, float *data2, uint32_t len1, uint32_t len2,
                float dry, float wet) {

  float *dest = *data1;
  if (!data2 || !len2 || !len1 || !data1) {
    printf("Cannot mix.\n");
    return 0;
  }

  for (uint32_t i = 0; i < len2 * 2; i++) {
    if (i >= len1)
      break;

    dest[i] = ((dest[i] * dry) + (data2[i] * wet));
  }

  return len1;
}
