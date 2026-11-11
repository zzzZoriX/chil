cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(chil VERSION 1.0.0)

set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
add_subdirectory(external/SDL2)

add_executable(chil
    src/main.cpp
    src/playlists/controller.cpp
    src/music/player.cpp
    src/cores/music/core.cpp
    src/cores/input/core.cpp
    src/files/chil-files.cpp
)

target_include_directories(chil PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/SDL2/include
)

if(WIN32)
    target_link_options(chil PRIVATE -static -static-libgcc -static-libstdc++)
    target_link_libraries(chil PRIVATE SDL2::SDL2main SDL2::SDL2-static)
else()
    target_link_options(chil PRIVATE -static-libgcc -static-libstdc++)
    target_link_libraries(chil PRIVATE SDL2::SDL2-static)
endif()

