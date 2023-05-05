cmake_minimum_required(VERSION 3.10)
project(vcs)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(vcs main.cpp vcs.cpp cli.cpp)