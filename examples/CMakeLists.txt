cmake_minimum_required(VERSION 3.18)
project(perlin-noise LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# This assumes that SFML is already installed on the system, and that CMake
# is using a toolchain that provides the ability to search for it. Consider
# using a C++ package manager such as vcpkg to handle this.
find_package(SFML CONFIG REQUIRED system window graphics)

add_executable(demo demo.cpp)
target_link_libraries(demo PUBLIC
    sfml-graphics
    sfml-window
    sfml-system
)
