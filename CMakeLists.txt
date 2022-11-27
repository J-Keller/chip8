cmake_minimum_required(VERSION 3.23)
project(chip8)

set(CMAKE_CXX_STANDARD 23)

add_executable(chip8 main.cpp components/Memory.cpp components/Memory.h components/Display.cpp components/Display.h components/CPU.cpp components/CPU.h components/Input.cpp components/Input.h)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(chip8 ${SDL2_LIBRARIES})
find_package(Threads REQUIRED)
target_link_libraries(chip8 Threads::Threads)