#include <midi_input.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
  midi_callback callback;
} Thread_data_T;

float note_freq(int noteId) {
  return 440.0f * pow(2, ((float)noteId - 69) / 12.0f);
}

static void *input_thread(void *ptr) {
  Thread_data_T *data = ptr;

  MidiData_T inpacket;

  char packet[6][3];

  FILE *f = fopen("/dev/midi2", "rb");

  if (f == 0) {
    printf("Cannot open\n");
    return 0;
  }

  // now just wait around for MIDI bytes to arrive and print them to screen.
  int i = 0;
  while (1) {
    memset(packet, 0, 3 * 7 * sizeof(char));
    size_t bytes_read = 0;

    char pack[3];
    bytes_read = fread(&pack, 3, 1, f);

    if (pack[0] == -112)
    {
      int note = pack[1];
      int velocity = pack[2];
      inpacket.id = note;
      inpacket.type = NOTE_ON;
      inpacket.pitch = note_freq(note);
      inpacket.velocity = velocity;

      data->callback(&inpacket);
    }

    if (pack[0] == -128)
    {
      int note = pack[1];
      inpacket.type = NOTE_OFF;
      inpacket.id = pack[1];
      inpacket.velocity = 0;
      inpacket.pitch = note_freq(note);
      data->callback(&inpacket);
    }

  }

  return 0;
}

pthread_t receive_midi(midi_callback callback) {
  pthread_t t;

  Thread_data_T *data = calloc(1, sizeof(Thread_data_T));
  data->callback = callback;

  if (pthread_create(&t, 0, input_thread, data)) {
    printf("Could not start midi input thread.\n");
  }

  return t;
}
