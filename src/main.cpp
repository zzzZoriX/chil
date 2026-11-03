#include "include/output/errors.h"
#include "include/output/nice.h"
#include "include/cores/music/core.h"
#include "include/cores/input/core.h"
#include "include/checker/chil-file-checker.h"
#include <SDL2/SDL.h>
#include <SDL_audio.h>


int main() {
    check();

    SDL_AudioDeviceID device; 

    try {
        if(SDL_Init(SDL_INIT_AUDIO) < 0) 
            throw error(std::format("SDL init failed: {}", SDL_GetError()), error::code::sdlerr);

        chil::nice(chil("SDL initialized successfully\n"));

        music_core m_core;
        input_core i_core{std::make_unique<music_core>(m_core)};

        i_core.start();
        
    }
    catch (const error& e) {
        error::panic(std::move(e));

        SDL_Quit();

        return 1;
    }

    SDL_CloseAudioDevice(device);
    SDL_Quit();

    return 0;
}
