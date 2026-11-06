#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include <optional>

#define playlists_dir_name "chil_playlists"
#define songs_to_add_dir_name "chil_to_add"


std::filesystem::path get_home_path(void);
void check(void);
std::optional<std::filesystem::path> get_in_dir(const bool file, const std::filesystem::path& path, const std::string& name);
std::vector<std::string> get_dirs(const std::filesystem::path& path); 
std::vector<std::string> get_files(const std::filesystem::path& path);
