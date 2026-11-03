#include "../include/playlists/controller.h"
#include <SDL_audio.h>
#include <filesystem>
#include <memory>
#include "../include/output/errors.h"

using namespace std::filesystem;


playlist::playlist() {
    path catalog_path{get_home_dir() / chil_playlists_catalog_name};

    if(!exists(catalog_path)) 
        create_directory(catalog_path);


    SDL_AudioSpec spec;
    SDL_zero(spec);

    spec.freq = 44100;
    spec.format = AUDIO_S16SYS;
    spec.channels = 2;
    spec.callback = NULL;
    spec.samples = 4096;

    device = SDL_OpenAudioDevice(nullptr, 0, &spec, nullptr, 0);
    if(device == 0) {
        SDL_Quit();
        throw error(std::format("Failed to open audio device: {}", SDL_GetError()), error::code::auderr);
    }
}

playlist::~playlist() {
    SDL_CloseAudioDevice(device);
}


void playlist::load(const std::string& name) {
    path catalog_path{get_home_dir() / chil_playlists_catalog_name / name};

    if(!exists(catalog_path)) 
        throw error("Playlist do not exist", error::code::plterr);  
    
    for(const auto entry: directory_iterator(catalog_path)) {
        if(is_regular_file(entry)) {
            path song_path{catalog_path /  entry};
            auto title{entry.path().filename().string()};

            list.emplace_back(music::parse(title), song_path, std::make_unique<SDL_AudioDeviceID>(device));
        }
    }
}

void playlist::add(const std::string& name) {
    //todo
}

void playlist::remove(const std::string& name) {
    //todo
}

void playlist::make(const std::string& name) {
    path catalog_path{get_home_dir() / chil_playlists_catalog_name / name};

    if(!exists(catalog_path))
        create_directory(catalog_path);
}

void playlist::play(const play_settings settings) {
    for(auto& song: list)
        song.play();
}

path playlist::get_home_dir(void) {
#if defined(_WIN32)
    const char* user_profile{std::getenv("USERPROFILE")};
    if(user_profile) {
        return fs::path(user_profile);
    }
    const char* home_drive{std::getenv("HOMEDRIVE")};
    const char* home_path{std::getenv("HOMEPATH")};
    if(home_drive && home_path) {
        return path(home_drive) / home_path;
    }
#else
    const char* home{std::getenv("HOME")};
    if(home) {
        return path(home);
    }
#endif

    return path();
}
