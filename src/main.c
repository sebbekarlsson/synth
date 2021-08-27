#include <audiomath.h>
#include <midi_input.h>
#include <notedefs.h>
#include <playback.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <synth.h>
#include <utils.h>

#define SAMPLE_RATE 48000

// NOTE: The clicking noise is from multiple notes being played very fast.

Oscillator_T *osc;
Oscillator_T *osc2;
Oscillator_T *osc3;

Device_T *device;

volatile int frame;

void callback(void *data) {
  MidiData_T *mdata = data;

  if (mdata->type == NOTE_ON && mdata->velocity) {

    printf("ON note: %12.6f\n", mdata->pitch);

    Note_T *note = device_send_note(
        device, (Note_T){mdata->id, mdata->pitch, mdata->velocity * 0.02f, 1});
  } else if (mdata->type == NOTE_OFF) {
    printf("OFF note: %12.6f\n", mdata->pitch);
    Note_T *note = device_send_note(
        device, (Note_T){mdata->id, mdata->pitch, mdata->velocity * 0.02f, 0});
  }
}

int main(int argc, char *argv[]) {

  pthread_t midi_thread = receive_midi(callback);

  osc = oscillator_init(OSC_PERLIN);
  osc2 = oscillator_init(OSC_COS);
  osc3 = oscillator_init(OSC_SIN);

  device = device_init(SAMPLE_RATE);
  device_add_oscillator(device, osc3);
  // device_add_oscillator(device, osc);

  // wav_write("audio.wav", dd, framebuffer->length, SAMPLE_RATE, 32, 2);

  int devid = 0;
  while (1) {

    AudioBuffer_T *framebuffer = device_next_frame(device, frame);

    if (framebuffer && framebuffer->data && framebuffer->length) {
      float *dd = smoothen(framebuffer->data, framebuffer->length, 1.0f);
      devid = play_audio(dd, framebuffer->length, SAMPLE_RATE);
    }

    frame += BLOCK_SIZE >> 3;
  }

  pthread_join(midi_thread, 0);

  return 0;
}
