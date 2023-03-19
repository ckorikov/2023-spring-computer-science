#include <boost/json/src.hpp>
#include <boost/locale.hpp>
#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "fmt/core.h"
#include "config.h"

#ifndef TGBOT_ENABLE_INLINE_KEYBOARD
#include "bot/bot.hpp"
#else
#include "bot/bot_inline_keyboard/bot_inline_keyboard.hpp"
#endif

#include "course/course.hpp"
#include "tgbot/tgbot.h"
#include "weather/weather.hpp"

using namespace std;
using namespace TgBot;

int main(int argc, char* argv[]) {
    // Bot setup
    boost::locale::generator gen;
    locale::global(gen(""));

    string bot_token_str;
    string weather_api_str;

    try {
        boost::program_options::options_description desc("Allowed Options");
        boost::program_options::variables_map vm;

        desc.add_options()("help,h", "Get allowed options")(
            "token,t", boost::program_options::value<string>()->required(),
            "Set path to bot_token.txt")(
            "api,a", boost::program_options::value<string>()->required(),
            "Set path to openweathermap_api.txt");

        boost::program_options::store(
            boost::program_options::parse_command_line(argc, argv, desc), vm);

        if (vm.count("help")) {
            cout << "Usage: tgbot [options...]\n\n" << desc;
            return 1;
        }

        if (vm.count("token")) {
            ifstream bot_token_file(vm["token"].as<string>());
            if (bot_token_file.is_open()) {
                bot_token_file >> bot_token_str;
            } else {
                fmt::print("Error: while opening file {}: {}\n", vm["token"].as<string>(), strerror(errno));
                return 1;
            }
        }

        if (vm.count("api")) {
            ifstream weather_api_file(vm["api"].as<string>());
            if (weather_api_file.is_open()) {
                weather_api_file >> weather_api_str;
            } else {
                fmt::print("Error: while opening file {}: {}\n", vm["api"].as<string>(), strerror(errno));
                return 1;
            }
        }

        boost::program_options::notify(vm);
    } catch (const exception& ex) {
        cout << "Error: " << ex.what() << '\n';
        return 1;
    }

#ifndef TGBOT_ENABLE_INLINE_KEYBOARD
    bot bot(bot_token_str, weather_api_str);
#else
    bot_inline_keyboard bot(bot_token_str, weather_api_str);
#endif

    try {
        bot.start();
    } catch (TgBot::TgException& ex) {
        cout << "Error: " << ex.what() << '\n';
    }

    return 0;
}
