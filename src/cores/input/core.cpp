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
    std::cout << "\tCHIL CLI\nfor help type 'info' or '10'\n" << std::endl;

    bool paused = false;

    while(true) {
        std::string act;

        std::cout << prompt;
        std::getline(std::cin, act); 

        auto number{get_number(act)};

        if(number < 0 || number > act_max) {
            warn::fear(warn("Unknown command. Type 10 / info for more information about CHIL\n"));

            continue;
        }

        switch(number) {
            case QUIT: // quit
                std::cout << "quiting..." << std::endl;
                return;

            case LOAD: { // load
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

            case PLAY: // play
                m_core->playlist_->play(play_settings());
                chil::nice(chil(std::format("Playlist {} starts playing\n", m_core->playlist_->name)));
                break;

            case PAUSE: // pause
                m_core->playlist_->pause();
                paused = !paused;

                chil::nice(chil(std::format("{}\n", (paused ? "Paused" : "Unpaused"))));
                break;

            case STOP: // stop
                m_core->playlist_->stop();
                chil::nice(chil("Playlist stoped\n"));
                break;

            case ADD: // add 
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

            case REMOVE: // remove
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

            case VOLUME:
                break;

            case LOOK:
                break;

            case CONFIG:
                break;

            case INFO: // info
                std::cout << CHIL_INFO << std::endl;
                break;
        }
    }
}

int input_core::get_number(const std::string& act) const {
    if(act == "load" || act == "1") return LOAD;
    if(act == "play" || act == "2") return PLAY;
    if(act == "pause" || act == "3") return PAUSE;
    if(act == "stop" || act == "4") return STOP;
    if(act == "add" || act == "5") return ADD;
    if(act == "remove" || act == "6") return REMOVE;
    if(act == "volume" || act == "7") return VOLUME;
    if(act == "look" || act == "8") return LOOK;
    if(act == "config" || act == "9") return CONFIG;
    if(act == "info" || act == "10") return INFO;
    if(act == "quit" || act == "0") return QUIT;

    return act_max + 1;
}
