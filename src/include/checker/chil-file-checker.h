#pragma once

#include <filesystem>

#define playlists_dir_name "chil_playlists"
#define songs_to_add_dir_name "chil_to_add"


std::filesystem::path get_home_path(void);
void check(void);
