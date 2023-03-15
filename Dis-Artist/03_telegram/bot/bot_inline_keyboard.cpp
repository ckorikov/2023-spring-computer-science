#include "bot/bot_inline_keyboard/bot_inline_keyboard.hpp"
#include "fmt/core.h"
#include "tgbot/TgException.h"
#include "tgbot/types/CallbackQuery.h"

bot_inline_keyboard::bot_inline_keyboard(const std::string& bot_token_str,
                                         const std::string& weather_api_str)
    : bot(bot_token_str, weather_api_str),
      keyboard_weather(new TgBot::InlineKeyboardMarkup),
      keyboard_course(new TgBot::InlineKeyboardMarkup),
      button_weather(new TgBot::InlineKeyboardButton),
      button_course(new TgBot::InlineKeyboardButton) {
    get_weather_with_buttons_ = false;
    get_course_with_buttons_ = false;

    button_weather->text = "Красноярск";
    button_weather->callbackData = "krasnoyarsk_weather";
    button_course->text = "USD";
    button_course->callbackData = "usd_valute";

    row_weather.push_back(button_weather);
    row_course.push_back(button_course);

    keyboard_weather->inlineKeyboard.push_back(row_weather);
    keyboard_course->inlineKeyboard.push_back(row_course);
}

void bot_inline_keyboard::start() {
    start_command();
    help_command();
    weather_command();
    course_command();
    check_input_keyboard();
    check_input();

    fmt::print("Bot username: {}\n", bot_.getApi().getMe()->username.c_str());
    bot_.getApi().deleteWebhook();
    while (true) {
        fmt::print("Long poll started\n");
        long_poll_.start();
    }
}

void bot_inline_keyboard::weather_command() {
    bot_.getEvents().onCommand("weather", [&](TgBot::Message::Ptr message) {
        bot_.getApi().sendMessage(message->chat->id, "Введите название города",
                                  false, 0, keyboard_weather, "Markdown");
        get_weather_city_ = true;
        long_poll_.start();
        if (get_weather_with_buttons_) {
            get_weather_city_ = false;
            get_weather_with_buttons_ = false;
        } else {
            weather_.refresh();

            if (weather_.check_city()) {
                bot_.getApi().sendMessage(
                    message->chat->id,
                    "Погода в городе: " + weather_.get_city() + '\n' +
                        weather_.get_weather() + "\nтемпература " +
                        std::to_string(weather_.get_temp()) + "°C\nветер " +
                        std::to_string(weather_.get_wind()) + " m/h");
                get_weather_city_ = false;
            } else {
                bot_.getApi().sendMessage(message->chat->id,
                                          "Введен неверный город");
                get_weather_city_ = false;
            }
        }
    });
}

void bot_inline_keyboard::course_command() {
    bot_.getEvents().onCommand("course", [&](TgBot::Message::Ptr message) {
        bot_.getApi().sendMessage(message->chat->id,
                                  "Введите валюту.\nНапример: usd", false, 0,
                                  keyboard_course, "Markdown");
        get_course_valute_ = true;
        long_poll_.start();
        if (get_course_with_buttons_) {
            get_course_valute_ = false;
            get_course_with_buttons_ = false;
        } else {
            course_.refresh();

            if (course_.check_valute()) {
                bot_.getApi().sendMessage(
                    message->chat->id,
                    course_.get_valute() + ": " +
                        std::to_string(course_.get_course()) + " ₽");
                get_course_valute_ = false;
            } else {
                bot_.getApi().sendMessage(message->chat->id,
                                          "Введена неверная валюта");
                get_course_valute_ = false;
            }
        }
    });
}

void bot_inline_keyboard::check_input_keyboard() {
    bot_.getEvents().onCallbackQuery([&](TgBot::CallbackQuery::Ptr query) {
        if (StringTools::startsWith(query->data, "krasnoyarsk_weather")) {
            get_weather_with_buttons_ = true;
            weather_.set_city("Красноярск");
            weather_.refresh();
            bot_.getApi().sendMessage(
                query->message->chat->id,
                "Погода в городе: " + weather_.get_city() + '\n' +
                    weather_.get_weather() + "\nтемпература " +
                    std::to_string(weather_.get_temp()) + "°C\nветер " +
                    std::to_string(weather_.get_wind()) + " m/h",
                false, 0, keyboard_weather, "Markdown");
        } else if (StringTools::startsWith(query->data, "usd_valute")) {
            get_course_with_buttons_ = true;
            course_.set_valute("USD");
            course_.refresh();
            bot_.getApi().sendMessage(
                query->message->chat->id,
                "USD : " + std::to_string(course_.get_course()) + " ₽", false,
                0, keyboard_course, "Markdown");
        }
    });
}
