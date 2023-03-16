#ifndef BOT_INLINE_KEYBOARD_HPP
#define BOT_INLINE_KEYBOARD_HPP

#include "bot/bot.hpp"
#include "course/course.hpp"
#include "tgbot/Bot.h"
#include "tgbot/net/TgLongPoll.h"
#include "tgbot/types/InlineKeyboardButton.h"
#include "tgbot/types/InlineKeyboardMarkup.h"
#include "weather/weather.hpp"

#include <string>
#include <vector>

class bot_inline_keyboard : public bot {
   public:
    bot_inline_keyboard(const std::string& bot_token_str,
                        const std::string& weather_api_str);

    void start() override;

   private:
    bool get_weather_with_buttons_;
    bool get_course_with_buttons_;

    TgBot::InlineKeyboardMarkup::Ptr keyboard_weather;
    TgBot::InlineKeyboardMarkup::Ptr keyboard_course;
    TgBot::InlineKeyboardButton::Ptr button_weather;
    TgBot::InlineKeyboardButton::Ptr button_course;
    std::vector<TgBot::InlineKeyboardButton::Ptr> row_weather;
    std::vector<TgBot::InlineKeyboardButton::Ptr> row_course;

    void weather_command() override;
    void course_command() override;
    void check_input_keyboard();
};

#endif
