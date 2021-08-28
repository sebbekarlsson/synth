#ifndef ROB_AUDIO_BUFFER_H
#define ROB_AUDIO_BUFFER_H
#include <stdint.h>

typedef struct {
  float *data;
  uint32_t length;
} AudioBuffer_T;

AudioBuffer_T *audio_buffer_init();

void audio_buffer_ensure(AudioBuffer_T *ab, uint32_t length);

AudioBuffer_T *audio_buffer_copy(AudioBuffer_T *ab);

#endif
