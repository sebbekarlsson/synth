#ifndef ROB_OSC_H
#define ROB_OSC_H
#include <stdint.h>
#include <types.h>
float *generate_wave(Oscillator_Type wave_type, float pitch, float velocity, float sample_rate, float sec, float phase_shift, uint32_t* length);
#endif
