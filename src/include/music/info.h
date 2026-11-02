#pragma once

#include <string>
#include <chrono>


struct music_info {
    std::string name;
    std::string author;
    std::chrono::seconds duration;
};
