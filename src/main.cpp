#include "include/output/errors.h"
#include "include/playlists/controller.h"
#include <SDL2/SDL.h>


int main(int argc, char** argv) {
    try {
        if(argc < 2)
            throw error("Too few arguments. Example of correct usage:\n> chil info", error::code::atferr);      

        if(SDL_Init(SDL_INIT_AUDIO) < 0) 
            throw error(std::format("SDL init failed: {}", SDL_GetError()), error::code::sdlerr);

        playlist pl;

        pl.load("govno");

        pl.play(play_settings());
    }
    catch (const error& e) {
        error::panic(e);
    }

    return 0;
}
