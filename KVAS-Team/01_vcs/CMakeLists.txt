cmake_minimum_required(VERSION 3.10)

project(vcs)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

set(SOURCES
    src/cli.cpp 
    src/vcs.cpp 
    src/utils.cpp
    main.cpp 
)

add_executable(vcs ${SOURCES})
