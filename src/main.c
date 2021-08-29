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

#define RECORD 1
#define NR_NOTES 88

double delta = 0.0f;

Note_T notes[NR_NOTES];

pthread_t midi_thread;

pthread_mutex_t note_lock;

double envelope_at(double x) {
  return MAX(0, MIN(10, x * x) - (MAX(0, x * 0.9f)));
}

double fabs(double x) {
  if (x < 0) return x * -1;
  return x;
}

double delay_f(double x) {
  double a = 4.0f;
  double b = 10.0f;
  //return MAX(sin(x) / TAU, MIN(1.0f, (double)MAX(0.0f, fabs((double)cos((double)(x/1.0f)*(double)a)))) - (x/b));
  //
  return MAX(0.0f, MIN(1.0f, 1 + (cos(x*4)/1.9f) - sqrt(x/7.0f)));
}

void midi_callback_func(void *ptr) {

  MidiData_T *data = ptr;

  pthread_mutex_lock(&note_lock);
  notes[data->note.id].id = data->note.id;

  if (data->type == NOTE_ON) {
    printf("ON\n");
    notes[data->note.id].created = delta;
    notes[data->note.id].velocity = data->note.velocity;
    notes[data->note.id].pitch = data->note.pitch;
    notes[data->note.id].created = delta;
    notes[data->note.id].on = 1;
  } else if (data->type == NOTE_OFF) {
    printf("OFF\n");
    notes[data->note.id].pitch = data->note.pitch;
    notes[data->note.id].on = 0;
  }
  pthread_mutex_unlock(&note_lock);
}

double get_sample(double delta) {
  double sample = 0;



  for (int i = 0; i < NR_NOTES; i++) {

    double created = notes[i].created;

    double env = 0.0f;

    double diff = delta - created;
    uint32_t delay_index = (int)ceil(diff/2);

    double delay_sample = delay_f(diff);

    double velocity =  notes[i].velocity * delay_sample;

    env = MAX(0.1f, envelope_at(diff * 10) * 0.5) * 0.1f;

    double harm1 = ((double)sin(notes[i].pitch * TAU * delta)) * env;
    double harm2 = ((double)sin((notes[i].pitch * TAU * delta) * 2)) * env;
    double harm3 = ((double)cos((notes[i].pitch * TAU * delta) * 3)) * env;
    double harm4 = ((double)sin((notes[i].pitch * TAU * delta) * 4)) * env;
    double nr_harms = 4;
    sample +=
        (((((harm1 + harm2 + harm3 + harm4) / nr_harms) * velocity) * 0.005f));
  }

  return sample;
}

pthread_t player_thread_id;
double player_delta = 0.0f;
void *player_func(void *ptr) {

  float *buff = (float *)calloc(BLOCK_SIZE, sizeof(float));
  int i = 0;

  float *recording = 0;
  uint32_t rec_len = 0;
  uint32_t sample_count = 0;
  uint32_t samples_per_second = 0;

  moment last_block_time = get_time();

  while (1) {
    if (i >= BLOCK_SIZE) {
      i = 0;

      play_audio(buff, (BLOCK_SIZE) * sizeof(float), SAMPLE_RATE);

      rec_len += BLOCK_SIZE;

      player_delta =
          player_delta + (double)((double)BLOCK_SIZE / (double)SAMPLE_RATE);
#if RECORD != 0
      recording = realloc(recording, rec_len * sizeof(float));
      memcpy(&recording[rec_len - BLOCK_SIZE], buff,
             BLOCK_SIZE * sizeof(float));

#endif
    } else {
      buff[i] = get_sample(delta);
      delta = delta + (double)((double)1.0f / (double)SAMPLE_RATE);
      sample_count += 1;
      i++;
    }

    if (time_diff_milli_s(last_block_time, get_time()) >= 1000) {
      samples_per_second = sample_count;
      sample_count = 0;
      last_block_time = get_time();
    }

    if (delta >= SAMPLE_RATE / 2) {
      delta = 0;
    }

#if RECORD != 0
    if (delta >= 30)
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
//  printf("Setting up midi...\n");
  //sleep(1);

  pthread_create(&player_thread_id, 0, player_func, 0);
  //printf("Setting up player...\n");
  //sleep(1);

  printf("Done!\n");

  midi_thread = receive_midi(midi_callback_func);

  pthread_join(player_thread_id, 0);

  return 0;

  pthread_join(midi_thread, 0);

  return 0;
}
