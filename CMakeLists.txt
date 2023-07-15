cmake_minimum_required(VERSION 3.25)
project(game_of_life)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)
include_directories(game_of_life ${SDL2_INCLUDE_DIRS})

add_executable(game_of_life main.cpp)
target_link_libraries(game_of_life ${SDL2_LIBRARIES})
