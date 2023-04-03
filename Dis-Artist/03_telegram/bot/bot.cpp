#include "bot/bot.hpp"
#include "fmt/core.h"
#include "tgbot/TgException.h"
#include "tgbot/types/BotCommand.h"

#include <boost/locale.hpp>
#include <cstdio>

bot::bot(const std::string& bot_token_str, const std::string& weather_api_str)
    : bot_(bot_token_str), long_poll_(bot_), weather_(weather_api_str) {
    get_weather_city_ = false;
    get_course_valute_ = false;

    bot_commands_ = {"start", "help", "weather", "course"};
    bot_commands_description_ = {"Начать общение", "Посмотреть что умеет бот",
                                 "Посмотреть прогноз погоды",
                                 "Посмотреть курс валюты"};

    std::vector<TgBot::BotCommand::Ptr> commands;
    std::vector<TgBot::BotCommand::Ptr> vectCmd;

    for (std::size_t x = 0; x != bot_commands_.size(); x++) {
        TgBot::BotCommand::Ptr cmdArray(new TgBot::BotCommand);
        cmdArray->command = bot_commands_[x];
        cmdArray->description = bot_commands_description_[x];
        commands.push_back(cmdArray);
    }

    bot_.getApi().setMyCommands(commands);
    vectCmd = bot_.getApi().getMyCommands();

    for (const auto& cmd : bot_.getApi().getMyCommands()) {
        fmt::print("cmd: {} -> {}\r", cmd->command.c_str(),
                   cmd->description.c_str());
    }
}

void bot::start() {
    start_command();
    help_command();
    weather_command();
    course_command();
    check_input();

    fmt::print("Bot username: {}\n", bot_.getApi().getMe()->username.c_str());
    bot_.getApi().deleteWebhook();
    while (true) {
        fmt::print("Long poll started\n");
        long_poll_.start();
    }
}

void bot::start_command() {
    bot_.getEvents().onCommand("start", [&](TgBot::Message::Ptr message) {
        bot_.getApi().sendMessage(message->chat->id,
                                  "Привет! Вызови /help и "
                                  "посмотри что я могу.");
    });
}

void bot::help_command() {
    bot_.getEvents().onCommand("help", [&](TgBot::Message::Ptr message) {
        bot_.getApi().sendMessage(
            message->chat->id,
            "Могу показать погоду по твоему городу. Вызови /weather"
            "\nМогу показать курс валюты которую хочешь "
            "Вызови /course");
    });
}

void bot::weather_command() {
    bot_.getEvents().onCommand("weather", [&](TgBot::Message::Ptr message) {
        bot_.getApi().sendMessage(message->chat->id, "Введите название города");

        get_weather_city_ = true;
        long_poll_.start();
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
    });
}

void bot::course_command() {
    bot_.getEvents().onCommand("course", [&](TgBot::Message::Ptr message) {
        bot_.getApi().sendMessage(message->chat->id,
                                  "Введите валюту.\nНапример: usd");

        get_course_valute_ = true;
        long_poll_.start();
        course_.refresh();

        if (course_.check_valute()) {
            bot_.getApi().sendMessage(message->chat->id,
                                      course_.get_valute() + ": " +
                                          std::to_string(course_.get_course()) +
                                          " ₽");
            get_course_valute_ = false;
        } else {
            bot_.getApi().sendMessage(message->chat->id,
                                      "Введена неверная валюта");
            get_course_valute_ = false;
        }
    });
}

void bot::check_input() {
    bot_.getEvents().onAnyMessage([&](TgBot::Message::Ptr message) {
        if (get_weather_city_) {
            weather_.set_city(message->text);
            return;
        }

        if (get_course_valute_) {
            course_.set_valute(boost::locale::to_upper(message->text));
            return;
        }

        for (const auto& command : bot_commands_) {
            if ("/" + command == message->text) {
                return;
            }
        }

        bot_.getApi().sendMessage(message->chat->id,
                                  "Не знаю такой команды. Вызови "
                                  "/help и посмотри что я могу.");
    });
}
