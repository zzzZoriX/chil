#include "../../include/cores/input/core.h"
#include <iostream>
#include <format>
#include "../../include/output/nice.h"
#include "../../include/output/warn.h"
#include "../../include/output/errors.h"
#include "../../include/files/chil-files.h"


input_core::input_core(std::unique_ptr<music_core> m_core)
:   m_core(std::move(m_core)) {}

input_core::~input_core(void) {
    if(t.joinable())
        t.join();
}


void input_core::start(void) {
    t = std::thread(&input_core::input_thread, this);
}

void input_core::input_thread(void) {
    std::cout << "\tCHIL CLI\nfor help enter '7'\n" << std::endl;

    int act;
    bool paused = false;

    while(true) {
        std::cout << prompt;
        std::cin >> act;

        if(act < 0) act = -act;
        if(act > act_max) act %= act_max;

        switch(act) {
            case 0: // quit
                std::cout << "quiting..." << std::endl;
                return;

            case 1: { // load
                        int selected;
                        auto playlists{playlist::get_playlists()};

                        if(playlists.empty()) {
                            std::cout << "No playlist in folder to play...\n" << std::endl;
                            break;
                        }

                        std::cout << "select playlist:" << std::endl;
                        for(int i{0}; i < playlists.size(); ++i)
                            std::cout << std::format("{}: {}", i, playlists.at(i)) << std::endl;

                        std::cout << prompt;
                        std::cin >> selected;

                        if(selected < 0 || selected > playlists.size()) {
                            warn::fear(warn("Unknown playlist"));
                            break;
                        }

                        m_core->playlist_->load(playlists.at(selected));
                        chil::nice(chil("Playlist loaded successfully\n"));
                    }
                break;

            case 2: // play
                m_core->playlist_->play(play_settings());
                chil::nice(chil(std::format("Playlist {} starts playing\n", m_core->playlist_->name)));
                break;

            case 3: // pause
                m_core->playlist_->pause();
                paused = !paused;

                chil::nice(chil(std::format("{}\n", (paused ? "Paused" : "Unpaused"))));
                break;

            case 4: // stop
                m_core->playlist_->stop();
                chil::nice(chil("Playlist stoped\n"));
                break;

            case 5: // add 
                try {
                    int selected;
                    const auto songs{get_files(get_home_path() / songs_to_add_dir_name)};

                    if(songs.empty()) {
                        std::cout << "No songs in folder to add...\n" << std::endl;
                        break;
                    }

                    std::cout << std::format("Select the song which will be added to {}:", m_core->playlist_->name) << std::endl;
                    for(int i{0}; i < songs.size(); ++i)
                        std::cout << std::format("{}: {}", i, songs.at(i)) << std::endl;

                    std::cout << prompt;
                    std::cin >> selected;

                    if(selected < 0 || selected >> songs.size())
                       throw error("Index out of range\n", error::code::argerr);

                    m_core->playlist_->add(songs.at(selected)); 
                    chil::nice(std::format("{} successfully added to {}\n", songs.at(selected), m_core->playlist_->name));
                }
                catch (const error& e) {
                    error::panic(std::move(e));
                }
                catch (const std::exception& e) {
                    error::panic(error(e.what(), error::code::auderr));
                    return;
                }
                break;

            case 6: // remove
                try {
                    int selected;
                    const auto songs{get_files(get_home_path() / playlists_dir_name / m_core->playlist_->name)};

                    if(songs.empty()) {
                        std::cout << std::format("No songs in {}...\n", m_core->playlist_->name) << std::endl;
                        break;
                    }

                    std::cout << "Select the song which will be moved to 'to add' folder:" << std::endl;
                    for(int i{0}; i < songs.size(); ++i)
                        std::cout << std::format("{}: {}", i, songs.at(i)) << std::endl;

                    std::cout << prompt;
                    std::cin >> selected;

                    if(selected < 0 || selected >> songs.size())
                       throw error("Index out of range\n", error::code::argerr);

                    m_core->playlist_->remove(songs.at(selected)); 
                    chil::nice(std::format("{} successfully moved\n", songs.at(selected)));
                }
                catch (const error& e) {
                    error::panic(std::move(e));
                }
                catch (const std::exception& e) {
                    error::panic(error(e.what(), error::code::auderr));
                    return;
                }
                break;

            case 7: // info
                std::cout << "info...\n" << std::endl;
                break;
        }
    }
}
