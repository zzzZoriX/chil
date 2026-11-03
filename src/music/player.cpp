#include "../include/music/player.h"
#include "../include/output/errors.h"


player::player(player& other) noexcept {
    this->device = std::move(other.device);

    this->buffer = std::move(other.buffer);
    other.buffer.clear();
    other.buffer.shrink_to_fit();

    this->pause_status = other.pause_status;
}

player::player(std::unique_ptr<SDL_AudioDeviceID> device)
:   device(std::move(device))
,   pause_status(1) {}

player::~player(void) {
    SDL_CloseAudioDevice(*device);
}


void player::load(const std::string& song) {
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
    pause_status = !pause_status;

    SDL_PauseAudioDevice(*device, pause_status);   
}

void player::play(void) {
    if(buffer.empty()) {
        SDL_Quit();
        throw error("No audio for decoding", error::code::auderr);
    }

    SDL_QueueAudio(*device, buffer.data(), buffer.size());
    pause();

    while(SDL_GetQueuedAudioSize(*device) > 0)
        SDL_Delay(100);
}
