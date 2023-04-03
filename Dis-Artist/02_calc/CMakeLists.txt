cmake_minimum_required(VERSION 3.11)
project(calc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# --- Fetch FTXUI --------------------------------------------------------------
include(FetchContent)

FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v3.0.0
)

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
# ------------------------------------------------------------------------------

add_executable(calc main.cpp src/ui.cpp src/logic.cpp)
target_include_directories(calc PRIVATE src)
target_link_libraries(calc ftxui::dom ftxui::screen ftxui::component)