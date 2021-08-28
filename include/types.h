#ifndef ROB_TYPES_H
#define ROB_TYPES_H
#include <audiobuffer.h>
#include <list.h>
#include <moment.h>

typedef struct {
  int id;
  float pitch;
  float velocity;
  float sustain;
  int on;
  unsigned int dead;
  double created;
} Note_T;

Note_T *note_init(int id, float pitch, float velocity, float sustain, int on,
                  double created);

void note_free(void *note);

typedef struct {
  float attack;
  float release;
  float sustain;
} Envelope_T;

typedef enum {
  OSC_SIN,
  OSC_COS,
  OSC_TAN,
  OSC_PERLIN,
  OSC_CUSTOM
} Oscillator_Type;

typedef struct OSCILLATOR_STRUCT {
  AudioBuffer_T buffer;
  Oscillator_Type type;
  float phase_shift;
} Oscillator_T;

Oscillator_T *oscillator_init(Oscillator_Type type);

#endif
