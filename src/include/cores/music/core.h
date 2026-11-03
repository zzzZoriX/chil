#pragma once

#include <SDL2/SDL.h>
#include "../../playlists/controller.h"


class music_core {
    SDL_AudioDeviceID device;
    std::unique_ptr<playlist> playlist_;

public:
    friend class input_core;

    music_core(music_core&);
    music_core(void);
    ~music_core(void);   

    void throw_exception(void) const;
};
