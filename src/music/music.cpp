#include "../include/music/music.h"
#include <SDL2/SDL.h>
#include "../include/output/errors.h"
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_error.h>
#include <format>
#include <vector>
#include <cstdio>
#include <sstream>
#include <iterator>


music::music(music_info info, path song_path, std::unique_ptr<SDL_AudioDeviceID> device) {
    this->info = info; 
    this->song_path = song_path;
    this->device = std::move(device);
}

music::~music() {
    device.reset();
}

void music::play(void) {
    read();

    SDL_QueueAudio(*device, buffer.data(), buffer.size());
    SDL_PauseAudioDevice(*device, 0);

    while(SDL_GetQueuedAudioSize(*device) > 0)
        SDL_Delay(100);

    SDL_CloseAudioDevice(*device);
    SDL_Quit();
}

music_info music::parse(const std::string& str) {
    std::istringstream iss{str};
    std::vector<std::string> results{
        std::istream_iterator<std::string>{iss},
        std::istream_iterator<std::string>{}
    };

    if(results.size() < 3) {
        throw std::runtime_error("Invalid music format: " + str);
    }

    std::chrono::seconds secs{std::stoll(results.at(2))};
    return {.name = results.at(0), .author = results.at(1), .duration = secs};
}

void music::read(void) {
    auto song{song_path.c_str()};

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

    if(buffer.empty()) {
        SDL_Quit();
        throw error("No audio for decoding", error::code::auderr);
    }
}
