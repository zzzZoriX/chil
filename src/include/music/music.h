#pragma once

#include <SDL_audio.h>
#include <filesystem>
#include <SDL2/SDL.h>

using namespace std::filesystem;


struct music {
    std::string name;
    path song_path;

    music(music&&) noexcept = default;
    music(const music& other) noexcept
    :   name(other.name)
    ,   song_path(other.song_path) {}
    music(const std::string& name, path song_path)
    :   name(name) 
    ,   song_path(song_path) {}
};
