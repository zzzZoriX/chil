#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <memory>
#include <string>
#include <mutex>
#include <thread>
#include <atomic>
#include "../output/errors.h"


class player {
    std::vector<char> buffer;
    std::unique_ptr<SDL_AudioDeviceID> device;
    int pause_status;
    std::atomic<bool> running;

    std::mutex mutex;
    std::thread t;
    error thread_error;

public:
    player(player&) noexcept;
    player(std::unique_ptr<SDL_AudioDeviceID> device);
    ~player(void);

    void play(void);
    void pause(void);
    void load(const std::string& song);
    void stop(void);

private:
    void player_thread(void);
};
