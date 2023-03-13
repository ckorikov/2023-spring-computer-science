cmake_minimum_required(VERSION 3.10)
project(vcs)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(vcs main.cpp src/vcs.cpp src/cli.cpp)
target_include_directories(vcs PRIVATE src)