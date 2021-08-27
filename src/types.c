#include <audiomath.h>
#include <osc.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <types.h>
#include <utils.h>
#include <wav.h>

pthread_mutex_t lock;

volatile int frame = 0;

AudioBuffer_T *audio_buffer_copy(AudioBuffer_T ab, uint32_t start,
                                 uint32_t len) {
  if (!len)
    return 0;
  AudioBuffer_T *audiobuffer = calloc(1, sizeof(AudioBuffer_T));
  audiobuffer->data = calloc((len), sizeof(float));

  for (uint32_t i = 0; i < len; i++) {
    audiobuffer->data[i] = ab.data[start + i];
  }

  audiobuffer->length = len;
  return audiobuffer;
}

Device_T *device_init(float sample_rate) {
  Device_T *device = (Device_T *)calloc(1, sizeof(Device_T));
  device->buffer = (AudioBuffer_T){0, 0};
  device->oscillators = list_init(sizeof(Oscillator_T));
  device->buffers = list_init(sizeof(AudioBuffer_T *));
  device->sample_rate = sample_rate;

  for (int i = 0; i < 88; i++) {
    device->notes[i] = (Note_T *)calloc(1, sizeof(Note_T));
    device->notes[i]->id = i;
    device->notes[i]->pitch = 0;
    device->notes[i]->velocity = 0;
    device->notes[i]->on = 0;
  }
  for (int i = 0; i < 88; i++) {
    device->note_times[i] = get_time();
  }

  device->last_note_time = get_time();

  device->envelope = (Envelope_T){0, 0.9f, 0};

  device->is_changed = 1;

  if (pthread_mutex_init(&lock, 0)) {
    printf("Device: mutex error.\n");
  }

  return device;
}

list_T *device_get_notes(Device_T *device) {
  list_T *list = list_init(sizeof(Note_T));

  for (int i = 0; i < 88; i++) {
    list_push(list, device->notes[i]);
  }

  return list;
}

AudioBuffer_T *note_compute(Note_T *note, Oscillator_T *oscillator,
                            float sample_rate) {
  if (!oscillator_compute(oscillator, note->pitch, note->velocity, sample_rate,
                          1.0f)) {
    printf("Unable to compute oscillator.\n");
    return 0;
  }
  return audio_buffer_copy(oscillator->buffer, 0, oscillator->buffer.length);
}

int device_compute(Device_T *device, uint32_t frame) {
  if (device->oscillators->size <= 0) {
    return 1;
  }

  float dry = 1.0f;
  float wet = (float)device->oscillators->size * (1.0f / 100.0f);

  float *note_mix_buffer = (float *)calloc(BLOCK_SIZE, sizeof(float));

  for (uint32_t i = 0; i < 88; i++) {
    Note_T *note = device->notes[i];

    for (uint32_t k = 0; k < device->oscillators->size; k++) {
      Oscillator_T *osc = (Oscillator_T *)list_at(device->oscillators, k);
      if (!osc)
        continue;

      AudioBuffer_T *note_buffer = note_compute(note, osc, device->sample_rate);

      if (!note_buffer) {
        printf("Could not compute note.\n");
        continue;
      };

      if (!wav_mix(note_mix_buffer, note_buffer->data, BLOCK_SIZE,
                   note_buffer->length, 1, wet)) {
        printf("device_compute: Could not mix.\n");
        continue;
      }

      free(note_buffer->data);
      free(note_buffer);
    }
  }

  AudioBuffer_T *ab = (AudioBuffer_T *)calloc(1, sizeof(AudioBuffer_T));
  float *copied_data = (float *)calloc(BLOCK_SIZE, sizeof(float));
  memcpy(copied_data, note_mix_buffer, BLOCK_SIZE * sizeof(float));
  ab->data = copied_data;
  ab->length = BLOCK_SIZE;
  list_push(device->buffers, ab);

  pthread_mutex_lock(&lock);
  device_update_midi_info(device);
  pthread_mutex_unlock(&lock);

  free(note_mix_buffer);

  device->is_changed = 0;
  return 0;
}

int device_clear_buffer(Device_T *device) {
  if (!device->buffer.data || !device->buffer.length)
    return 1;
  free(device->buffer.data);
  device->buffer.data = 0;
  device->buffer.length = 0;

  return 0;
}

void device_add_oscillator(Device_T *device, Oscillator_T *oscillator) {
  list_push(device->oscillators, oscillator);
}

Note_T *device_send_note(Device_T *device, Note_T note) {

  pthread_mutex_lock(&lock);

  if (!note.id) {
    printf("device_send_note: Note has not id.\n");
    return 0;
  }

  Note_T *heap_note = device->notes[note.id];

  heap_note->pitch = note.pitch;
  heap_note->velocity = note.velocity;
  heap_note->on = note.on;

  device->notes[note.id] = heap_note;
  printf("ID: %d %d\n", heap_note->id, heap_note->on);

  device->is_changed = 1;

  pthread_mutex_unlock(&lock);

  return heap_note;
}

void device_update_midi_info(Device_T *device) {
  Note_T *heap_note = 0;
  for (int i = 0; i < 88; i++) {
    heap_note = device->notes[i];

    if (heap_note->on != 1)
      heap_note->velocity =
          MAX(0, heap_note->velocity - device->envelope.release);
  }
}

AudioBuffer_T *device_next_frame(Device_T *device, uint32_t frame) {
  device_compute(device, frame);
  AudioBuffer_T *buffer = list_pop(device->buffers);
  if (!buffer)
    return 0;
  return (AudioBuffer_T *)buffer;
}
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
