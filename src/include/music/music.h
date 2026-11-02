#pragma once

#include "info.h"
#include <SDL_audio.h>
#include <filesystem>
#include <SDL2/SDL.h>
#include <memory>
#include <vector>

using namespace std::filesystem;


class music {
    music_info info;
    path song_path;

    std::unique_ptr<SDL_AudioDeviceID> device;
    std::vector<char> buffer;

public:
    music(music&&) noexcept = default;
    music(music_info info, path song_path, std::unique_ptr<SDL_AudioDeviceID> device);
    ~music();
    void play(void);

    static music_info parse(const std::string& str);

private:
    void read(void);
};
