#pragma once

#include <SDL_audio.h>
#include <filesystem>
#include <SDL2/SDL.h>

using namespace std::filesystem;


struct music {
    std::string name;
    path song_path;

    music(music&&) noexcept = default;
    music(music&) noexcept = default;
    music(const std::string& name, path song_path)
    :   name(name) 
    ,   song_path(song_path) {}
};
