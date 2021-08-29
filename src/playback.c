#include <SDL2/SDL.h>
#include <audiomath.h>
#include <playback.h>
#include <stdio.h>

int is_init = 0;

SDL_AudioDeviceID deviceId;

int play_audio(float *buffer, uint32_t length, float sample_rate) {

  int len = 128;
  int frame = 0;
  // int len = ;
  if (!is_init) {
    printf("Audio init.\n");
    SDL_Init(SDL_INIT_AUDIO);

    SDL_AudioSpec want;
    SDL_memset(&want, 0, sizeof(want));
    want.freq = (int)sample_rate;
    want.format = AUDIO_F32SYS;
    want.channels = 2;
    want.samples = len;
    want.callback = 0; // my_audio_callback;

    SDL_AudioSpec have = {};

    deviceId = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);

    SDL_PauseAudioDevice(deviceId, 0);

    is_init = 1;

    SDL_Delay(60);
  }

  if (!length || buffer == 0) {
    printf("play_audio: no audio to play.\n");
    return 1;
  }

  //  uint32_t iterations = length / 2;
  SDL_QueueAudio(deviceId, buffer, length);
  SDL_Delay((float)(len/2) / (float)sample_rate);

  //  SDL_Delay(((double)length/2.0f) / (double)SAMPLE_RATE);
  while (SDL_GetQueuedAudioSize(deviceId)) {
  }

  /*  for (int i = 0; frame < iterations; i++) {
     SDL_QueueAudio(deviceId, &buffer[frame], len >> 1);

     frame += len >> 3;
    }*/

  //  SDL_CloseAudioDevice(deviceI9;

  return deviceId;
}
