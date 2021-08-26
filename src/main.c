#include <midi_input.h>
#include <notedefs.h>
#include <stdio.h>
#include <synth.h>
#include <utils.h>

#define SAMPLE_RATE 48000

void callback(void *data) {
  MidiData_T *mdata = data;

  if (mdata->type == NOTE_ON) {
    printf("ON note: %12.6f\n", mdata->pitch);
  } else {
    printf("OFF note: %12.6f\n", mdata->pitch);
  }
}

int main(int argc, char *argv[]) {

  pthread_t t = receive_midi(callback);

  pthread_join(t, 0);

  return 0;
  int wave_length = 0;

  Oscillator_T *osc = oscillator_init(OSC_PERLIN);
  Oscillator_T *osc2 = oscillator_init(OSC_COS);
  Oscillator_T *osc3 = oscillator_init(OSC_SIN);

  Device_T *device = device_init(SAMPLE_RATE);

  device_add_oscillator(device, osc);
  device_add_oscillator(device, osc2);
  device_add_oscillator(device, osc3);

  Note_T *note = device_send_note(device, (Note_T)NOTE_C2);

  if (device_compute(device)) {
    printf("Unable to compute device.\n");
    return 1;
  }

  if (!device->buffer.data || !device->buffer.length) {
    printf("Device buffer is empty :(\n");
    return 1;
  }

  float *b = smoothen(device->buffer.data, device->buffer.length, 1.0f);

  printf("Buffer length: %d\n", device->buffer.length);

  wav_write("audio.wav", b, device->buffer.length, SAMPLE_RATE, 32, 2);

  return 0;
}
