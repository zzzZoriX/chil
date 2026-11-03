#pragma once

#include "info.h"
#include <SDL_audio.h>
#include <filesystem>
#include <SDL2/SDL.h>

using namespace std::filesystem;


struct music {
    music_info info;
    path song_path;

    music(music&&) noexcept = default;
    music(music&) noexcept = default;
    music(music_info info, path song_path);

    static music_info parse(const std::string& str);
};
