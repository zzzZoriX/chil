#include "../../include/cores/input/core.h"
#include <iostream>
#include <format>
#include "../../include/output/nice.h"
#include "../../include/output/warn.h"
#include "../../include/output/errors.h"


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
                m_core->playlist_->add("");
                break;

            case 6: // remove
                m_core->playlist_->remove("");
                break;

            case 7: // info
                std::cout << "info...\n" << std::endl;
                break;
        }
    }
}
