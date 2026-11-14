cmake_minimum_required(VERSION 3.15)

project(chil VERSION 1.0.0 LANGUAGES C CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_TEST OFF CACHE BOOL "" FORCE)

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
)

target_compile_definitions(chil PRIVATE
    SDL_MAIN_HANDLED
)

target_link_libraries(chil PRIVATE
    SDL2::SDL2-static
)

if(WIN32)
    target_link_options(chil PRIVATE
        -mconsole
        -static
        -static-libgcc
        -static-libstdc++
    )
elseif(UNIX)
    target_link_options(chil PRIVATE
        -static-libgcc
        -static-libstdc++
    )
endif()

set_target_properties(chil PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
