#include <audiomath.h>
#include <math.h>
#include <osc.h>
#include <perlin.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double perl(double v) { return perlin_get2d(v, 0, 3.3f, 6.0f, 342342); }

typedef double (*wave_gen_F)(double v);

float *generate_wave(Oscillator_Type wave_type, float pitch, float velocity,
                     float sample_rate, float sec, float phase_shift,
                     uint32_t *length) {
  if (!sample_rate) {
    printf("generate_wave: sample_rate is zero.\n");
    return (void *)0;
  }

  pitch = pitch > 0 ? pitch : 440.0f;
  float seconds = (sample_rate * (sec * 1000.0f) / 60.0f) / 2;

  float cycle = floor(((float)((float)sample_rate / (float)pitch)));
  float samples_left = seconds;
  int nr_cycles = (float)samples_left / (float)cycle;

  if (!nr_cycles) {
    printf("generate_wave: nr_cycles is zero.\n");
    return (void *)0;
  }

  uint32_t memsize = (uint32_t)ceil(seconds);

  float *buffer = (float *)calloc((seconds), sizeof(float));
  // memset(buffer, 0, memsize*sizeof(float));
  if (buffer == 0) {
    printf("generate_wave: Could not allocate buffer.\n");
    return (void *)0;
  }

  *length = seconds;

  float volume = velocity;

  wave_gen_F fptr = 0;

  switch (wave_type) {
  case OSC_COS:
    fptr = cos;
    break;
  case OSC_TAN:
    fptr = tan;
    break;
  case OSC_SIN:
    fptr = sin;
    break;
  case OSC_PERLIN:
    fptr = perl;
    break;
  case OSC_CUSTOM:
    fptr = 0;
    break; // not implemented yet
  default:
    fptr = 0;
    break;
  }

  if (!fptr) {
    printf("generate_wave: No wave gen function found for wave_type: %d\n",
           wave_type);
    return (void *)0;
  }

  for (uint32_t i = 0; i < seconds; i++) {
    buffer[i] =
        (volume * fptr(((float)(i + phase_shift) / (float)cycle) * TAU));
  }

  return buffer;
}
