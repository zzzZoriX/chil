#pragma once

#include "../music/core.h"
#include <memory>
#include <thread>

#define QUIT 0
#define LOAD 1
#define PLAY 2
#define PAUSE 3
#define STOP 4
#define ADD 5
#define REMOVE 6
#define VOLUME 7
#define LOOK 8
#define CONFIG 9
#define INFO 10

#define prompt "> "
#define act_max 10
#define CHIL_INFO \
    "\t\t\t\t\tCHIL\n" \
    "\tby zorix\n" \
    "\tVersion: 1.0.0\n" \
    "\n" \
    "\tA simple application for creating playlists and enjoying your music collection.\n" \
    "\n" \
    "\tCOMMANDS:\n" \
    "\n" \
    "\t[0] quit     - Exit the application.\n" \
    "\t[1] load     - Load a playlist from the playlists directory (default: chil_playlists).\n" \
    "\t[2] play     - Start playing the loaded playlist.\n" \
    "\t[3] pause    - Pause or resume the current track.\n" \
    "\t[4] stop     - Stop playlist playback.\n" \
    "\t[5] volume   - Adjust the playback volume.\n" \
    "\t[6] add      - Add a track from the incoming directory (default: chil_to_add) to the loaded playlist directory. Reload the playlist using 'load' to make the track available.\n" \
    "\t[7] remove   - Move a track from the loaded playlist directory back to the incoming directory.\n" \
    "\t[8] look     - Display tracks from the loaded playlist (only tracks present when the playlist was loaded).\n" \
    "\t[9] config   - Open configuration settings.\n" \
    "\t[10] info    - Display this information.\n"


class input_core {
    std::unique_ptr<music_core> m_core;
    std::thread t;

public:
    input_core(std::unique_ptr<music_core> m_core);
    ~input_core(void);

    void start(void);

private:
    void input_thread(void);
    int get_number(const std::string& act) const;
};
