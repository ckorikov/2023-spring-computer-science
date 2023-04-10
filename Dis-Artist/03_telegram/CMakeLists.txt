find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(CURL REQUIRED)
find_package(Boost 1.75.0 REQUIRED COMPONENTS system program_options locale REQUIRED)

set(
    SRC_LIST
    main.cpp 
    network/network.cpp 
    weather/weather.cpp
    course/course.cpp
    bot/bot.cpp
)

set(
    LIB_LIST
    PRIVATE TgBot
    PRIVATE fmt
    ${CMAKE_THREAD_LIBS_INIT} 
    ${OPENSSL_LIBRARIES} 
    ${Boost_LIBRARIES} 
    ${CURL_LIBRARIES}
)

set(
    FLAG_LIST
    -Wall 
    -Werror 
    -Wextra 
    -Winit-self 
    -Wredundant-decls
    -Wpedantic 
    -flto=auto -O3
)

include_directories(
    ${OPENSSL_INCLUDE_DIR} 
    ${Boost_INCLUDE_DIR}
)

if(TGBOT_ENABLE_INLINE_KEYBOARD)
    set(SRC_LIST ${SRC_LIST} bot/bot_inline_keyboard/bot_inline_keyboard.cpp)
    add_definitions(-DTGBOT_ENABLE_INLINE_KEYBOARD)
endif()

add_executable(
    ${PROJECT_NAME} 
    ${SRC_LIST}
)

target_compile_options(
    ${PROJECT_NAME} PRIVATE
    ${FLAG_LIST}
)

target_link_libraries(
    ${PROJECT_NAME} 
    ${LIB_LIST}
)

include(GNUInstallDirs)
install(
    TARGETS ${PROJECT_NAME}        
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
