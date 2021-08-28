#include <audiobuffer.h>
#include <mem.h>
#include <stdio.h>
#include <stdlib.h>

AudioBuffer_T *audio_buffer_init() {
  AudioBuffer_T *ab = (AudioBuffer_T *)scalloc(1, sizeof(AudioBuffer_T));
  ab->data = 0;
  ab->length = 0;

  return ab;
}

AudioBuffer_T *audio_buffer_copy(AudioBuffer_T *ab) {
  AudioBuffer_T *audiobuffer = audio_buffer_init();
  audiobuffer->data = scalloc((ab->length), sizeof(float));

  for (uint32_t i = 0; i < ab->length; i++) {
    audiobuffer->data[i] = ab->data[i];
  }

  audiobuffer->length = ab->length;
  return audiobuffer;
}

void audio_buffer_ensure(AudioBuffer_T *ab, uint32_t length) {
  if (!ab) {
    printf("AudioBuffer is null.\n");
    exit(1);
  }
  if (ab->data == 0 && ab->length > 0) {
    printf("Audiobuffer has no data, but length > 0. Terminating.\n");
    exit(1);
  }

  if (ab->data == 0) {
    ab->data = (float *)scalloc(length, sizeof(float));
    ab->length = length;
  }
}
