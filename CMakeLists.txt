cmake_minimum_required(VERSION 3.29)
project(MP3)

set(CMAKE_CXX_STANDARD 20)

add_executable(MP3 main.cpp
        mp3tag.cpp)
