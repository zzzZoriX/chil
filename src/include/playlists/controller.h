#pragma once

#include <string>
#include "settings.h"
#include "../music/music.h"
#include <vector>
#include <SDL2/SDL.h>
#include <filesystem>

#define chil_playlists_catalog_name "cplc"

using namespace std::filesystem;


class playlist {
    std::string name;
    std::vector<music> list;

    SDL_AudioDeviceID device;

public:
    playlist();
    ~playlist();

    void load(const std::string& name);
    void make(const std::string& name);
    void add(const std::string& name);
    void remove(const std::string& name);
    void play(const play_settings settings);

private:
    path get_home_dir(void);
};
