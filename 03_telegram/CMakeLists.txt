cmake_minimum_required(VERSION 3.11)
project(telegram)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# --- Fetch tgbot-cpp ----------------------------------------------------------
include(FetchContent)

FetchContent_Declare(
  tgbot-cpp
  GIT_REPOSITORY https://github.com/reo7sp/tgbot-cpp.git
  GIT_TAG master
)

FetchContent_MakeAvailable(tgbot-cpp)
# ------------------------------------------------------------------------------

find_package(Boost)

add_executable(telegram main.cpp config.cpp)
target_link_libraries(telegram TgBot Boost::boost)