#ifndef ROB_MIX_H
#define ROB_MIX_H
#include <stdint.h>

int mix_buffers(float **data1, float *data2, uint32_t len1, uint32_t len2,
                float dry, float wet);
#endif
