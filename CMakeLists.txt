cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(chil VERSION 1.0.0)

find_package(SDL2 REQUIRED)

add_executable(chil src/main.cpp src/music/music.cpp src/playlists/controller.cpp)

target_include_directories(chil PRIVATE ${SDL2_INCLUDE_DIRS})

target_link_libraries(chil PRIVATE ${SDL2_LIBRARIES})
