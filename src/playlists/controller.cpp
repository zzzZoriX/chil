#include "../include/playlists/controller.h"
#include <SDL_audio.h>
#include <filesystem>
#include <memory>
#include "../include/output/errors.h"

using namespace std::filesystem;


playlist::playlist() {
    path catalog_path{absolute(chil_playlists_catalog_name)};

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
    path catalog_path{absolute(chil_playlists_catalog_name)};
    catalog_path += name;

    if(!exists(catalog_path)) 
        throw error("Playlist do not exist", error::code::plterr);  

    auto end{catalog_path.end()};
    
    for(auto it{catalog_path.begin()}; it != end; ++it) {
        auto title{it->c_str()};
        auto song_path{catalog_path};
        song_path += title;

        list.emplace_back(music::parse(title), song_path, std::make_unique<SDL_AudioDeviceID>(device));
    }
}

void playlist::add(const std::string& name) {
    //todo
}

void playlist::remove(const std::string& name) {
    //todo
}

void playlist::make(const std::string& name) {
    path catalog_path{absolute(chil_playlists_catalog_name)};
    catalog_path += name;

    if(!exists(catalog_path))
        create_directory(catalog_path);
}

void playlist::play(const play_settings settings) {
    for(auto& song: list)
        song.play();
}
