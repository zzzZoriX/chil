#pragma once

#include <SDL2/SDL.h>
#include <SDL_audio.h>
#include <vector>
#include <memory>
#include <string>
#include <mutex>
#include <thread>
#include <atomic>
#include <optional>
#include "../output/errors.h"


class player {
    std::vector<char> buffer;
    std::unique_ptr<SDL_AudioDeviceID> device;
    int pause_status;
    std::atomic<bool> running;

    std::mutex mutex;
    std::thread t;
    error thread_error;

    std::string song_name;
    int volume;

public:
    friend class playlist;
    friend class music_core;

    player(player&) noexcept;
    player() = default;
    player(std::unique_ptr<SDL_AudioDeviceID> device);
    ~player(void);

    player& operator=(player&&);

    void play(void);
    void pause(void);
    void load(const std::string& song, const std::string& song_name);
    void stop(void);
    void set_volume(const int volume_percentage);

    std::string currently_playing(void) const;
    bool alive(void) const;
    std::optional<error> get_error(void) const;

    static void SDLCALL audio_callback(void* ud, Uint8* stream, int len); 

private:
    void player_thread(void);
    void handle_audio_callback(Uint8* stream, int len);
};
