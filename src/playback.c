#include <SDL2/SDL.h>
#include <audiomath.h>
#include <playback.h>
#include <stdio.h>

int is_init = 0;

SDL_AudioDeviceID deviceId;

int play_audio(float *buffer, uint32_t length, float sample_rate) {

  // int len = ;
  if (!is_init) {
    SDL_Init(SDL_INIT_AUDIO);

    SDL_AudioSpec want;
    SDL_memset(&want, 0, sizeof(want));
    want.freq = (int)sample_rate;
    want.format = AUDIO_F32SYS;
    want.channels = 2;
    want.samples = 512;
    want.callback = 0; // my_audio_callback;

    SDL_AudioSpec have = {};

    deviceId = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);

    SDL_PauseAudioDevice(deviceId, 0);

    is_init = 1;
  }

  if (!length || buffer == 0) {
    printf("play_audio: no audio to play.\n");
    return 1;
  }

  int frame = 0;
  uint32_t iterations = length;
  SDL_QueueAudio(deviceId, buffer, length * sizeof(float));

  while (SDL_GetQueuedAudioSize(deviceId)) {
  };

  // for (int i = 0; frame < iterations; i++) {
  // SDL_QueueAudio(deviceId, &buffer[frame], len >> 1);

  // frame += len >> 3;
  // }

  //  SDL_CloseAudioDevice(deviceId);

  return deviceId;
}
