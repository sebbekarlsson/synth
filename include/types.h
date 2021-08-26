#ifndef ROB_TYPES_H
#define ROB_TYPES_H
#include <list.h>

typedef struct {
  float* data;
  uint32_t length;
} AudioBuffer_T;

typedef struct {
  float id;
  float pitch;
  float velocity;
} Note_T;

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
  Envelope_T envelope;
  AudioBuffer_T buffer;
  Oscillator_Type type;
  float phase_shift;
} Oscillator_T;

Oscillator_T* oscillator_init(Oscillator_Type type);

int oscillator_compute(Oscillator_T* oscillator, float pitch, float velocity, float sample_rate, float duration);

typedef struct {
  list_T* notes;
} DeviceInput_T;

typedef struct {
  list_T* oscillators;
  DeviceInput_T input;
  AudioBuffer_T buffer;
  float sample_rate;
} Device_T;

int device_compute(Device_T* device);

Device_T* device_init(float sample_rate);

Note_T* device_send_note(Device_T* device, Note_T note);

void device_add_oscillator(Device_T* device, Oscillator_T* oscillator);
#endif
