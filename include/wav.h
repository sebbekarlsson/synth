#ifndef BOUNCE_H
#define BOUNCE_H
#include <stdint.h>

typedef struct WAV_HEADER {
  uint8_t riff[4];
  uint32_t overall_size;
  uint8_t wave[4];
  uint8_t fmt_chunk_marker[4];
  uint32_t length_of_fmt;
  uint16_t format_type;
  uint16_t channels;
  uint32_t sample_rate;
  uint32_t byterate;
  uint16_t block_align;
  uint16_t bits_per_sample;
  uint8_t data_chunk_header[4];
  uint32_t data_size;
} WavHeader;


typedef struct {
  unsigned char* data;
  uint32_t size;
  uint32_t size_b;
  uint32_t seek_p;
} IOFile;

int wav_write(const char* path, float* data, uint32_t size, float sample_freq,
              uint16_t bits_per_sample, uint16_t nr_channels);


int wav_mix(float** data1, float* data2, uint32_t len1, uint32_t len2, float dry, float wet);

#endif
