#include <stdlib.h>
#include <string.h>
#include <utils.h>

float *convolve(float *invec, int32_t len, float *kernel, int K, float mix) {

  float *outvec = (float *)calloc(len, sizeof(float));
  for (size_t i = K / 2; i < len - K / 2; ++i) {
    outvec[i] = 0.0;
    for (size_t j = 0; j < K + 1; j++) {
      outvec[i - K / 2] += invec[i - K / 2 + j] * (kernel[j] * mix);
    }
  }

  return outvec;
}

float *smoothen(float *data, int32_t data_len, float mix) {
  float kernel[15] = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.5f, 0.6f, 1.0f,
                      0.6f, 0.5f, 0.5f, 0.4f, 0.3f, 0.2f, 0.1f};

  float *newarr = (float *)calloc(data_len, sizeof(float));
  memcpy(newarr, &data[0], data_len * sizeof(float));

  return convolve(newarr, data_len, kernel, 15, mix);
}
