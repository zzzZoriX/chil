#include "../include/music/player.h"
#include "../include/output/errors.h"
#include <SDL_audio.h>
#include <mutex>
#include <thread>
#include <algorithm>


player::player(player& other) noexcept {
    std::lock_guard lock{other.mutex};

    this->device = std::move(other.device);

    this->buffer = std::move(other.buffer);
    other.buffer.clear();
    other.buffer.shrink_to_fit();

    this->pause_status = other.pause_status;
    this->running = other.running.exchange(false);

    this->t = std::move(other.t);
    this->thread_error = other.thread_error;
    this->song_name = other.song_name;
    this->volume = other.volume;

    other.thread_error = error();
    other.song_name.clear();
    other.song_name.shrink_to_fit();
}

player::player(std::unique_ptr<SDL_AudioDeviceID> device)
:   device(std::move(device))
,   pause_status(0)
,   running(true)
,   volume(SDL_MIX_MAXVOLUME) {}

player::~player(void) {
    if(t.joinable())
        t.join();
}

player& player::operator=(player&& other) {
    if(this != &other) {
        stop();

        std::lock_guard lock{mutex},
                        lock2{other.mutex};

        this->buffer = other.buffer;
        this->device = std::move(other.device);
        this->pause_status = other.pause_status;
        this->running = other.running.exchange(false);
        this->t = std::move(other.t);
        this->thread_error = other.thread_error;
        this->song_name = other.song_name;
        this->volume = other.volume;

        other.buffer.clear();
        other.buffer.shrink_to_fit();
        other.pause_status = 1;
        other.thread_error = error();
        other.song_name.clear();
        other.song_name.shrink_to_fit();
    }

    return *this;
}


void player::load(const std::string& song, const std::string& song_name) {
    this->song_name = song_name;

    if(!buffer.empty()) {
        buffer.clear();
        buffer.shrink_to_fit();
    } 

    std::string command{"ffmpeg -i \"" + std::string(song) + "\" -f s16le -acodec pcm_s16le -ar 44100 -ac 2 -loglevel quiet -"};

    FILE* pipe{popen(command.c_str(), "r")};
    if(!pipe) {
        SDL_Quit();
        throw error("Failed to open decoding pipe", error::code::auderr);
    }

    char chunk[4096];
    std::size_t bytes_read;

    while((bytes_read = fread(chunk, 1, sizeof(chunk), pipe)) > 0)
        buffer.insert(buffer.end(), chunk, chunk + bytes_read);

    pclose(pipe);
}

void player::pause(void) {
    std::lock_guard lock{mutex};

    pause_status = !pause_status;

    SDL_PauseAudioDevice(*device, pause_status);   
}

void player::play(void) {
    if(running.load())
        stop();

    running.store(true);
    pause_status = 0;

    t = (std::thread(&player::player_thread, this));
}

void player::stop(void) {
    running.store(false);

    {
        std::lock_guard lock{mutex};

        SDL_PauseAudioDevice(*device, 1);
        SDL_ClearQueuedAudio(*device);
    }

    if(t.joinable())
        t.join();
}

void player::player_thread(void) {
    {
        std::lock_guard lock{mutex};

        if(buffer.empty()) {
            SDL_Quit();
            thread_error = error("No audio for decoding", error::code::auderr);
            return;
        }

        SDL_QueueAudio(*device, buffer.data(), buffer.size());
        SDL_PauseAudioDevice(*device, pause_status);   
    }

    while(true) {
        std::size_t size{0};

        {
            std::lock_guard lock{mutex};

            size = SDL_GetQueuedAudioSize(*device);
        }

        if(size == 0 || !running.load())
            break;

        SDL_Delay(100);
    }

    running.store(false);
}

std::string player::currently_playing(void) const {
    return song_name;
}

bool player::alive(void) const {
    return running.load();
}

std::optional<error> player::get_error(void) const {
    if(std::string(thread_error.what()) != "noerr")
     return thread_error;   

    return std::nullopt;
}

void SDLCALL player::audio_callback(void* ud, Uint8* stream, int len) {
    player* player_ptr{static_cast<player*>(ud)};
    player_ptr->handle_audio_callback(stream, len);
}

void player::handle_audio_callback(Uint8* stream, int len) {
    SDL_memset(stream, 0, len);

    std::lock_guard lock{mutex};

    if(pause_status == 1 || buffer.empty())
        return;

    std::size_t bytes_to_copy{std::min(static_cast<std::size_t>(len), buffer.size())};


    SDL_MixAudioFormat(stream, reinterpret_cast<Uint8*>(buffer.data()), AUDIO_S16SYS, bytes_to_copy, volume);

    buffer.erase(buffer.begin(), buffer.begin() + bytes_to_copy);
}

void player::set_volume(const int volume_percentage) {
    std::lock_guard lock{mutex};

    volume = (std::clamp(volume_percentage, 0, 100) * SDL_MIX_MAXVOLUME) / 100;
}
