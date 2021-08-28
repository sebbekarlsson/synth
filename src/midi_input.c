#include <math.h>
#include <midi_input.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MIDI_INPUT "/dev/midi2"

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

  FILE *f = fopen(MIDI_INPUT, "rb");

  if (f == 0) {
    printf("Cannot open\n");
    return 0;
  }

  while (1) {
    memset(packet, 0, 3 * 7 * sizeof(char));

    char pack[3];
    fread(&pack, 3, 1, f);

    if (pack[0] == -112) {
      int note = pack[1];
      int velocity = pack[2];
      inpacket.type = NOTE_ON;
      inpacket.note = (Note_T){};
      inpacket.note.id = note;
      inpacket.note.pitch = note_freq(note);
      inpacket.note.velocity = velocity;

      data->callback(&inpacket);
    }

    if (pack[0] == -128) {
      int note = pack[1];
      inpacket.type = NOTE_OFF;
      inpacket.note = (Note_T){};
      inpacket.note.id = pack[1];
      inpacket.note.velocity = 0;
      inpacket.note.pitch = note_freq(note);
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
