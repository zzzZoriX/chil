#include "../include/files/chil-files.h"
#include <filesystem>
#include <string>
#include <iostream>
#include "../include/output/warn.h"
#include "../include/output/nice.h"

using namespace std::filesystem;


path get_home_path(void) {
#if defined(_WIN32)
    const char* user_profile{std::getenv("USERPROFILE")};
    if(user_profile) {
        return fs::path(user_profile);
    }
    const char* home_drive{std::getenv("HOMEDRIVE")};
    const char* home_path{std::getenv("HOMEPATH")};
    if(home_drive && home_path) {
        return path(home_drive) / home_path;
    }
#else
    const char* home{std::getenv("HOME")};
    if(home) {
        return path(home);
    }
#endif

    return path();
}

void check(void) {
    std::cout << "chil checks for the existence of folders...\n" << std::endl;

    auto playlists_dir{get_home_path() / playlists_dir_name};
    auto to_add{get_home_path() / songs_to_add_dir_name};   
    int warns_count{0};

    if(!exists(playlists_dir)) {
        warn::fear(warn("Playlist directory does not exists.\nchil will create this directory, don't worry\n"));

        create_directory(playlists_dir);

        ++warns_count;
    }

    if(!exists(to_add)) {
        warn::fear(warn("Directory for songs to add to playlists does not exists.\nchil will create this directory, don't worry\n"));   

        create_directory(to_add);

        ++warns_count;
    }

    if(warns_count == 0)
        chil::nice(chil("All directories pass the check successfully\n"));
}

std::optional<std::filesystem::path> get_in_dir(const bool file, const std::filesystem::path& path, const std::string& name) {
    for(const auto& entry:directory_iterator(path))
       if(
               (file ? is_regular_file(entry) : is_directory(name))
               && entry.path().filename().string() == name
         )
          return entry.path(); 

    return std::nullopt;
}

std::vector<std::string> get_dirs(const std::filesystem::path& path) {
    if(!is_directory(path) || !exists(path)) 
        return {};

    std::vector<std::string> dirs_names;

    for(const auto& entry: directory_iterator(path)) {
        if(is_directory(entry))
            dirs_names.push_back(entry.path().filename().string());
    }

    return dirs_names;
}

std::vector<std::string> get_files(const std::filesystem::path& path) {
    if(!is_directory(path) || !exists(path))
        return {};

    std::vector<std::string> files_names;

    for(const auto& entry: directory_iterator(path)) {
        if(is_regular_file(entry))
            files_names.push_back(entry.path().filename().string());
    }

    return files_names;
}
