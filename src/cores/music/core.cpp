#include "../../include/cores/music/core.h"
#include <memory>


music_core::music_core(music_core& other) {
    this->device = std::move(other.device);
    this->playlist_ = std::move(other.playlist_);
}

music_core::music_core(void) {
    SDL_AudioSpec spec;
    SDL_zero(spec);

    spec.freq = 44100;
    spec.format = AUDIO_S16SYS;
    spec.channels = 2;
    spec.callback = nullptr;
    spec.samples = 4096;

    device = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0);
    if(device == 0)
        throw error("Can't open audio device", error::code::auderr);

    auto temp_player{std::make_unique<player>(std::make_unique<SDL_AudioDeviceID>(device))};
    playlist_ = std::make_unique<playlist>(std::move(temp_player));
}

music_core::~music_core(void) {
    SDL_CloseAudioDevice(device);
}
