#pragma once

#include <memory>
#include <string>
#include "settings.h"
#include "../music/music.h"
#include "../music/player.h"
#include <vector>
#include <SDL2/SDL.h>
#include <mutex>
#include <thread>
#include <atomic>

#define chil_playlists_catalog_name "cplc"

using namespace std::filesystem;


class playlist {
    std::vector<music> list;

    std::shared_ptr<class player> player;

    std::thread t;
    error thread_error;
    std::mutex mutex;
    std::atomic<bool> playing;

public:
    std::string name;

    playlist() = default;
    playlist(std::shared_ptr<class player> player);
    ~playlist(void);

    playlist& operator=(playlist&&);

    void load(const std::string& name);
    void make(const std::string& name);
    void add(const std::string& name);
    void remove(const std::string& name);
    void play(const play_settings settings);
    void stop(void);
    void pause(void);
    static std::vector<std::string> get_playlists(void);
    std::vector<music> get_playlist(void) const noexcept;

private:
    void playlist_thread(void);
};
