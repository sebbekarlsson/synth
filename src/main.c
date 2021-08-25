#include <notedefs.h>
#include <stdio.h>
#include <synth.h>

#define SAMPLE_RATE 48000

int main(int argc, char *argv[]) {
  int wave_length = 0;

  Oscillator_T *osc = oscillator_init(OSC_TAN);
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

  printf("Buffer length: %d\n", device->buffer.length);

  wav_write("audio.wav", device->buffer.data, device->buffer.length,
            SAMPLE_RATE, 32, 2);

  return 0;
}
