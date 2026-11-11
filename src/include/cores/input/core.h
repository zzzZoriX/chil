#pragma once

#include "../music/core.h"
#include <memory>
#include <thread>

#define QUIT 0
#define LOAD 1
#define MAKE 2
#define PLAY 3
#define PAUSE 4
#define STOP 5
#define ADD 6
#define REMOVE 7
#define VOLUME 8
#define LOOK 9
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
    "\t[1] load     - Load a playlist from the playlists directory).\n" \
    "\t[2] make     - Make a playlist in the playlists directory"\
    "\t[3] play     - Start playing the loaded playlist.\n" \
    "\t[4] pause    - Pause or resume the current track.\n" \
    "\t[5] stop     - Stop playlist playback.\n" \
    "\t[6] volume   - Adjust the playback volume.\n" \
    "\t[7] add      - Add a track from the incoming directory to the loaded playlist directory. Reload the playlist using 'load' to make the track available.\n" \
    "\t[8] remove   - Move a track from the loaded playlist directory back to the incoming directory.\n" \
    "\t[9] look     - Display tracks from the loaded playlist (only tracks present when the playlist was loaded).\n" \
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
