#include "../include/files/parser.h"
#include "../include/output/errors.h"
#include "../include/music/info.h"


music parse(const std::filesystem::path& path) {
    TagLib::FileRef f{path.filename().string().c_str()};

    if(f.isNull() || !f.tag())
        throw error("Error: Could not read audio file or metadata metadata tags.", error::code::auderr);

    TagLib::Tag* tag{f.tag()};
    std::string name{tag->title().to8Bit(true)};
    std::string author{tag->artist().to8Bit(true)};

    TagLib::AudioProperties* properties{f.audioProperties()};
    int dur{0};
    
    if(properties)
        dur = properties->lengthInSeconds();

    music_info i{name, author, std::chrono::seconds(dur)};

    return music(i, path);
}

std::string convert_to_name(const music_info& info) {
    return std::format("{} {} {}", info.name, info.author, info.duration);
}
