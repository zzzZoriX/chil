#include "../../include/cores/music/core.h"
#include <memory>


music_core::music_core(void) {
    player_ = std::make_shared<player>(std::make_unique<SDL_AudioDeviceID>());
    playlist_ = std::make_unique<playlist>(player_);

    SDL_AudioSpec want, have;
    SDL_zero(want);

    want.freq = 44100;
    want.format = AUDIO_S16SYS;
    want.channels = 2;
    want.callback = nullptr;
    want.samples = 4096;

    want.callback = player::audio_callback;
    want.userdata = player_.get();

    device = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);
    if(device == 0)
        throw error("Can't open audio device", error::code::auderr);

    *(player_->device) = device;
}

music_core::~music_core(void) {
    SDL_CloseAudioDevice(device);
}
