#include "include/output/errors.h"
#include "include/cores/music/core.h"
#include "include/cores/input/core.h"
#include <SDL2/SDL.h>
#include <SDL_audio.h>


int main() {
    SDL_AudioDeviceID device; 

    try {
        if(SDL_Init(SDL_INIT_AUDIO) < 0) 
            throw error(std::format("SDL init failed: {}", SDL_GetError()), error::code::sdlerr);

        music_core m_core;
        input_core i_core{std::make_unique<music_core>(m_core)};

        i_core.start();
        
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
