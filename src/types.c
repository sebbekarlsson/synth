#include <osc.h>
#include <stdio.h>
#include <stdlib.h>
#include <types.h>
#include <wav.h>

Device_T *device_init(float sample_rate) {
  Device_T *device = (Device_T *)calloc(1, sizeof(Device_T));
  device->input = (DeviceInput_T){list_init(sizeof(Note_T))};
  device->buffer = (AudioBuffer_T){0, 0};
  device->oscillators = list_init(sizeof(Oscillator_T));
  device->sample_rate = sample_rate;

  return device;
}

int device_compute(Device_T *device) {
  if (device->input.notes->size <= 0 || device->oscillators->size <= 0)
    return 1;

  float dry = 1.0f;
  float wet = (float)device->oscillators->size * (1.0f / 100.0f);

  for (uint32_t i = 0; i < device->input.notes->size; i++) {
    Note_T *note = (Note_T *)list_at(device->input.notes, i);
    if (!note)
      continue;

    for (uint32_t k = 0; k < device->oscillators->size; k++) {
      Oscillator_T *osc = (Oscillator_T *)list_at(device->oscillators, k);
      if (!osc)
        continue;
      printf("--->%d\n", osc->type);

      if (!oscillator_compute(osc, note->pitch, note->velocity,
                              device->sample_rate, 1)) {
        printf("Deivce %p: Unable to compute oscillator %p.\n", &device, &osc);
        continue;
      }

      if (!(device->buffer.length =
                wav_mix(&device->buffer.data, osc->buffer.data,
                        device->buffer.length, osc->buffer.length, dry, wet))) {
        printf("Device %p: Unable to mix buffers.\n", &device);
      }
    }
  }

  return 0;
}

void device_add_oscillator(Device_T *device, Oscillator_T *oscillator) {
  list_push(device->oscillators, oscillator);
}

Note_T *device_send_note(Device_T *device, Note_T note) {
  Note_T *heap_note = (Note_T *)calloc(1, sizeof(Note_T));
  if (!heap_note)
    return 0;

  heap_note->id = note.id;
  heap_note->pitch = note.pitch;
  heap_note->velocity = note.velocity;

  list_push(device->input.notes, heap_note);

  return heap_note;
}
Oscillator_T *oscillator_init(Oscillator_Type type) {
  Oscillator_T *oscillator =
      (Oscillator_T *)calloc(1, sizeof(struct OSCILLATOR_STRUCT));
  oscillator->envelope = (Envelope_T){0, 0, 0};
  oscillator->buffer = (AudioBuffer_T){0, 0};
  oscillator->type = type;
  oscillator->phase_shift = 0;

  return oscillator;
}

int oscillator_compute(Oscillator_T *oscillator, float pitch, float velocity,
                       float sample_rate, float duration) {

  oscillator->buffer.data =
      generate_wave(oscillator->type, pitch, velocity, sample_rate, duration,
                    oscillator->phase_shift, &oscillator->buffer.length);

  return (oscillator->buffer.data != 0 && oscillator->buffer.length >= 0);
}
