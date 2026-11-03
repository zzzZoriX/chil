#include "../include/music/music.h"
#include <vector>
#include <sstream>
#include <iterator>


music::music(music_info info, path song_path) {
    this->info = info; 
    this->song_path = song_path;
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
