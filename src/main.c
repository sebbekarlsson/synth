#include <audiobuffer.h>
#include <audiomath.h>
#include <list.h>
#include <math.h>
#include <midi_input.h>
#include <playback.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utils.h>
#include <wav.h>

#define RECORD 0
#define NR_NOTES 88

double delta = 0.0f;

Note_T notes[NR_NOTES];

pthread_t midi_thread;

pthread_mutex_t note_lock;

void midi_callback_func(void *ptr) {

  MidiData_T *data = ptr;

  if (data->type == NOTE_ON) {
    printf("ON\n");
    notes[data->note.id].velocity = data->note.velocity;
    notes[data->note.id].pitch = data->note.pitch;
  } else if (data->type == NOTE_OFF) {
    printf("OFF\n");
    notes[data->note.id].velocity = 0.0f;
    notes[data->note.id].pitch = data->note.pitch;
  }
}

double get_sample(double delta) {
  double sample = 0;

  for (int i = 0; i < NR_NOTES; i++) {
    float harm1 = ((double)sin(notes[i].pitch * TAU * delta));
    float harm2 = ((double)cos(notes[i].pitch * TAU * (delta * 2)));
    float harm3 = (((double)sin(notes[i].pitch * TAU * (delta * 3))));
    float harm4 = (((double)sin((notes[i].pitch * 2) * TAU * (delta * 4))));
    sample +=
        (((harm1 + harm2 + harm3 + harm4) / 4.0f) * notes[i].velocity) * 0.005f;
  }

  return sample;
}

pthread_t player_thread_id;
void *player_func(void *ptr) {

  float *buff = (float *)calloc(BLOCK_SIZE, sizeof(float));
  int i = 0;

  float *recording = 0;
  uint32_t rec_len = 0;

  while (1) {
    if (i >= BLOCK_SIZE) {
      i = 0;
      play_audio(buff, BLOCK_SIZE * sizeof(float), SAMPLE_RATE);
      rec_len += BLOCK_SIZE;

#if RECORD != 0
      recording = realloc(recording, rec_len * sizeof(float));
      memcpy(&recording[rec_len - BLOCK_SIZE], buff,
             BLOCK_SIZE * sizeof(float));
#endif
    } else {
      buff[i] = get_sample(delta);
      i++;
    }
    delta += ((double)1.0f / (double)SAMPLE_RATE);

#if RECORD != 0
    if (delta >= 60)
      break;
#endif
  }

  wav_write("audio.wav", recording, rec_len * sizeof(float), SAMPLE_RATE, 32,
            2);

  return 0;
}

int main(int argc, char *argv[]) {

  for (int i = 0; i < NR_NOTES; i++) {
    notes[i] = (Note_T){i, 0, 0, 0, 0, 0, 0};
  }

  pthread_mutex_init(&note_lock, 0);
  pthread_create(&player_thread_id, 0, player_func, 0);
  midi_thread = receive_midi(midi_callback_func);

  pthread_join(player_thread_id, 0);

  return 0;

  pthread_join(midi_thread, 0);

  return 0;
}
