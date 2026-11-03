#include "include/output/errors.h"
#include "include/playlists/controller.h"
#include "include/music//player.h"
#include <SDL2/SDL.h>
#include <SDL_audio.h>


int main() {
    SDL_AudioDeviceID device; 

    try {
        if(SDL_Init(SDL_INIT_AUDIO) < 0) 
            throw error(std::format("SDL init failed: {}", SDL_GetError()), error::code::sdlerr);


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

        class player player{std::make_unique<SDL_AudioDeviceID>(device)};

        playlist pl{std::make_unique<class player>(player)};

        pl.load("govno");

        pl.play(play_settings());
    }
    catch (const error& e) {
        error::panic(e);

        SDL_Quit();

        return 1;
    }

    SDL_CloseAudioDevice(device);
    SDL_Quit();

    return 0;
}
