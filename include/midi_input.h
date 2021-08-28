#ifndef ROB_MIDI_INPUT_H
#define ROB_MIDI_INPUT_H
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/soundcard.h>
#include <types.h>
#include <unistd.h>

//#define  MIDI_DEVICE  "/dev/midi2"
#define MIDI_DEVICE "/dev/sequencer"

typedef enum {
  NOTE_OFF,
  NOTE_ON,
  NOTE_KEY,
  NOTE_CONTROLLER,
  NOTE_PROGRAM,
  NOTE_PRESSURE,
  NOTE_PITCHBEND
} EMidiEvent;

typedef struct {
  char noteoff[3];
  char noteon[3];
  char key[3];
  char controller[3];
  char program[3];
  char pressure[3];
  char pitchbend[3];
  EMidiEvent type;
} M_T;

typedef struct {
  EMidiEvent type;
  Note_T note;
} MidiData_T;

typedef void (*midi_callback)(void *ptr);

pthread_t receive_midi(midi_callback callback);

float note_freq(int noteId);

#endif
