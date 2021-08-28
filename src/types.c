#include <audiomath.h>
#include <math.h>
#include <osc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <types.h>
#include <utils.h>
#include <wav.h>

Oscillator_T *oscillator_init(Oscillator_Type type) {
  Oscillator_T *oscillator =
      (Oscillator_T *)calloc(1, sizeof(struct OSCILLATOR_STRUCT));
  oscillator->buffer = (AudioBuffer_T){0, 0};
  oscillator->type = type;
  oscillator->phase_shift = 0;

  return oscillator;
}

int oscillator_compute(Oscillator_T *oscillator, float pitch, float velocity,
                       float sample_rate, float duration) {

  if (oscillator->buffer.data) {
    free(oscillator->buffer.data);
    oscillator->buffer.data = 0;
    oscillator->buffer.length = 0;
  }
  oscillator->buffer.data =
      generate_wave(oscillator->type, pitch, velocity, sample_rate, duration,
                    oscillator->phase_shift, &oscillator->buffer.length);

  return (oscillator->buffer.data != 0 && oscillator->buffer.length >= 0);
}

Note_T *note_init(int id, float pitch, float velocity, float sustain, int on,
                  double created) {
  Note_T *note = (Note_T *)calloc(1, sizeof(Note_T));
  note->id = id;
  note->pitch = pitch;
  note->velocity = velocity;
  note->sustain = sustain;
  note->dead = 0;
  note->created = created;

  return note;
}

void note_free(void *noteptr) {
  Note_T *note = (Note_T *)noteptr;
  free(note);
}
