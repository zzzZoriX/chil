#pragma once

#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/audioproperties.h>
#include "../music/music.h"
#include <filesystem>


music parse(const std::filesystem::path& path);
std::string convert_to_name(const music_info& info);
