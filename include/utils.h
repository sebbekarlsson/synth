#ifndef ROB_UTILS_H
#define ROB_UTILS_H
#include <stdint.h>
#include <sys/param.h>

float *convolve(float *invec, int32_t len, float *kernel, int K, float mix);
float *smoothen(float *data, int32_t data_len, float mix);

#endif
