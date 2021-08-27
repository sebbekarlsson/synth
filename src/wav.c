#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wav.h>

IOFile *new_io_file() {
  IOFile *f = (IOFile *)calloc(1, sizeof(IOFile));
  f->size = 0;
  f->data = 0;
  f->size_b = 0;
  f->seek_p = 0;

  return f;
}

uint32_t io_write_int1(IOFile *f, unsigned char *bytes, uint32_t len) {
  len = len + 1;
  f->size += len;
  f->size_b += len * sizeof(unsigned char);

  f->data = realloc(f->data, f->size_b);
  memset(&f->data[f->seek_p], 0, len * sizeof(unsigned char));

  memcpy(&f->data[f->seek_p], &bytes[0], len * sizeof(bytes[0]));

  f->seek_p += len * sizeof(unsigned char);

  return f->size;
}

uint32_t io_write_int8(IOFile *f, uint8_t *bytes, uint32_t len) {
  f->size += len;
  f->size_b += len * sizeof(uint8_t);

  f->data = realloc(f->data, f->size_b);
  memset(&f->data[f->seek_p], 0, len * sizeof(uint8_t));

  memcpy(&f->data[f->seek_p], &bytes[0], len * sizeof(bytes[0]));

  f->seek_p += len * sizeof(uint8_t);

  return f->size;
}

uint32_t io_write_int16(IOFile *f, uint16_t *bytes, uint32_t len) {
  f->size += len;
  f->size_b += len * sizeof(uint16_t);

  f->data = realloc(f->data, f->size_b);
  memset(&f->data[f->seek_p], 0, len * sizeof(uint16_t));
  memcpy(&f->data[f->seek_p], &bytes[0], len * sizeof(bytes[0]));

  f->seek_p += len * sizeof(uint16_t);
  return f->size;
}

uint32_t io_write_int32(IOFile *f, uint32_t *bytes, uint32_t len) {
  f->size += len;
  f->size_b += len * sizeof(uint32_t);

  f->data = realloc(f->data, f->size_b);
  memset(&f->data[f->seek_p], 0, len * sizeof(uint32_t));
  memcpy(&f->data[f->seek_p], &bytes[0], len * sizeof(&bytes[0]));

  f->seek_p += len * sizeof(uint32_t);

  return f->size;
}

int wav_write(const char *path, float *data, uint32_t size, float sample_freq,
              uint16_t bits_per_sample, uint16_t nr_channels) {
  WavHeader header = {};

  FILE *out = fopen(path, "wb");
  if (out == NULL) {
    printf("Could not write to %s\n", path);
    return 0;
  }

  // IOFile* f = new_io_file();

  uint32_t byterate = sample_freq * nr_channels * (bits_per_sample / 8);
  uint64_t blockalign = (bits_per_sample / 8) * nr_channels;

  // printf("%d\n", sizeof(header));

  // fwrite(&f->data[0], 1, f->size_b, out);

  strncpy(header.riff, "RIFF", 4);
  strncpy(header.wave, "WAVE", 4);
  strncpy(header.fmt_chunk_marker, "fmt ", 4);
  strncpy(header.data_chunk_header, "data", 4);

  header.length_of_fmt = 16;
  header.format_type = 3;
  header.channels = nr_channels;
  header.sample_rate = sample_freq;
  header.bits_per_sample = bits_per_sample;
  header.byterate =
      header.sample_rate * header.channels * (header.bits_per_sample / 8);
  header.block_align = (header.bits_per_sample / 8) * header.channels;
  header.data_size = sizeof(&data[0]) * (size >> 3);
  header.overall_size = (header.data_size);

  printf("Writing file: %s\n", path);

  fwrite(&header, sizeof(header), 1, out);

  fseek(out, 0, SEEK_END);

  fwrite(&data[0], sizeof(&data[0]), size, out);

  fclose(out);

  printf("Done, wrote file %s\n", path);

  return 1;
}

int wav_mix(float *data1, float *data2, uint32_t len1, uint32_t len2, float dry,
            float wet) {
  if (!data2 || !len2 || !len1 || !data1) {
    printf("Cannot mix.\n");
    return 0;
  }

  for (uint32_t i = 0; i < len2; i++) {
    if (i >= len1)
      break;

    data1[i] = (data1[i] * dry) + (data2[i] * wet);
  }

  return len1;
}
