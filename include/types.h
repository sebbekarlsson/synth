#ifndef ROB_TYPES_H
#define ROB_TYPES_H
#include <list.h>
#include <moment.h>

typedef struct {
  float* data;
  uint32_t length;
} AudioBuffer_T;


AudioBuffer_T* audio_buffer_copy(AudioBuffer_T ab, uint32_t start, uint32_t end);

typedef struct {
  int id;
  float pitch;
  float velocity;
  int on;
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
  Note_T* notes[88];
  moment note_times[88];
  AudioBuffer_T buffer;
  float sample_rate;
  list_T* buffers;
  Envelope_T envelope;
  moment last_note_time;
  int is_changed;
} Device_T;

list_T* device_get_notes(Device_T* device);

int device_compute(Device_T* device, uint32_t frame);

AudioBuffer_T* device_next_frame(Device_T* device, uint32_t frame);

int device_clear_buffer(Device_T* device);

void device_update_midi_info(Device_T* device);

Device_T* device_init(float sample_rate);

Note_T* device_send_note(Device_T* device, Note_T note);

void device_add_oscillator(Device_T* device, Oscillator_T* oscillator);
#endif
