#ifndef BOT_HPP
#define BOT_HPP

#include "course/course.hpp"
#include "tgbot/Bot.h"
#include "tgbot/net/TgLongPoll.h"
#include "tgbot/types/InlineKeyboardMarkup.h"
#include "weather/weather.hpp"

#include <string>

class bot {
   public:
    bot(const std::string& bot_token_str, const std::string& weather_api_str);

    virtual void start();

   protected:
    void start_command();
    void help_command();
    virtual void weather_command();
    virtual void course_command();
    void check_input();

    TgBot::Bot bot_;
    TgBot::TgLongPoll long_poll_;
    bool get_weather_city_;
    bool get_course_valute_;
    std::vector<std::string> bot_commands_;
    std::vector<std::string> bot_commands_description_;
    weather weather_;
    course course_;
};

#endif
