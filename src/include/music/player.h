#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <memory>
#include <string>


class player {
    std::vector<char> buffer;
    std::unique_ptr<SDL_AudioDeviceID> device;
    int pause_status;

public:
    player(player&) noexcept;
    player(std::unique_ptr<SDL_AudioDeviceID> device);
    ~player(void);

    void play(void);
    void pause(void);
    void load(const std::string& song);
};
