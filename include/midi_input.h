#ifndef ROB_MIDI_INPUT_H
#define ROB_MIDI_INPUT_H
#include <sys/soundcard.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

//#define  MIDI_DEVICE  "/dev/midi2"
#define  MIDI_DEVICE  "/dev/sequencer"

typedef enum {
  NOTE_OFF,
  NOTE_ON,
  NOTE_KEY,
  NOTE_CONTROLLER,
  NOTE_PROGRAM,
  NOTE_PRESSURE,
  NOTE_PITCHBEND
} EMidiEvent;

typedef struct  {
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
  float pitch;
  float id;
  float velocity;
} MidiData_T;


typedef void (*midi_callback)(void* ptr);

pthread_t receive_midi(midi_callback callback);

#endif
