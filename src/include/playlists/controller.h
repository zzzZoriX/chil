#pragma once

#include <string>
#include "settings.h"
#include "../music/music.h"
#include "../music/player.h"
#include <vector>
#include <SDL2/SDL.h>
#include <filesystem>
#include <mutex>
#include <thread>
#include <atomic>

#define chil_playlists_catalog_name "cplc"

using namespace std::filesystem;


class playlist {
    std::string name;
    std::vector<music> list;

    std::unique_ptr<class player> player;

    std::thread t;
    error thread_error;
    std::mutex mutex;
    std::atomic<bool> playing;

public:
    playlist(std::unique_ptr<class player> player);
    ~playlist(void);

    void load(const std::string& name);
    void make(const std::string& name);
    void add(const std::string& name);
    void remove(const std::string& name);
    void play(const play_settings settings);
    void stop(void);

private:
    path get_home_dir(void);
    void playlist_thread(void);
};
