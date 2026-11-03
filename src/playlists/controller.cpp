#include "../include/playlists/controller.h"
#include <SDL_audio.h>
#include <filesystem>
#include <memory>
#include "../include/output/errors.h"

using namespace std::filesystem;


playlist::playlist(std::unique_ptr<class player> player) {
    path catalog_path{get_home_dir() / chil_playlists_catalog_name};

    if(!exists(catalog_path)) 
        create_directory(catalog_path);

    this->player = std::move(player);
}

playlist::~playlist(void) {
    if(t.joinable())
        t.join();

    stop();
}


void playlist::load(const std::string& name) {
    path catalog_path{get_home_dir() / chil_playlists_catalog_name / name};

    if(!exists(catalog_path)) 
        throw error("Playlist do not exist", error::code::plterr);  
    
    for(const auto entry: directory_iterator(catalog_path)) {
        if(is_regular_file(entry)) {
            path song_path{catalog_path /  entry};
            auto title{entry.path().filename().string()};

            list.emplace_back(music::parse(title), song_path);
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
    playing.store(true);
    t = std::thread(&playlist::playlist_thread, this);
}

void playlist::stop(void) {
    playing.store(false);   

    {
        std::lock_guard lock{mutex};
    
        player->stop();   

        if(std::string(thread_error.what()) != "noerr") 
            throw thread_error;
    }
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

void playlist::playlist_thread(void) {
    std::vector<music>::iterator song;

    {
        std::lock_guard lock{mutex};
    
        song = list.begin();
    }

    while(song != list.end() && playing.load()) {
        {
            std::lock_guard lock{mutex};
        
            player->load(song->song_path.c_str(), song->info.name);
            player->play();
        }

        while(true) {
            bool is_alive;

            {
                std::lock_guard lock{mutex};

                is_alive = player->alive();
            }

            if(!is_alive) break;
        }

        {
            std::lock_guard lock{mutex};

            if(auto err {player->get_error()}; err.has_value()) {
                thread_error = error(err->what(), error::code::auderr);
                return;
            }       
        }

        ++song;
    }
}
