#include <tgbot/tgbot.h>
#include <iostream>

#include "config.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Error! Missing config file path" << std::endl;
        return 1;
    }

    try
    {
        const auto token = config::get_token(argv[1]);
        auto bot = TgBot::Bot(token);

        // Bot config
        bot.getEvents().onCommand(
            "start", [&bot](TgBot::Message::Ptr message)
            { bot.getApi().sendMessage(message->chat->id, "Hi!"); });

        bot.getEvents().onAnyMessage([&bot](TgBot::Message::Ptr message)
                                     {
            std::cout << ">> " << message->text.c_str() << std::endl;
            if (StringTools::startsWith(message->text, "/start")) {
                return;
            }
            bot.getApi().sendMessage(message->chat->id, "Your message is: " + message->text); });

        // Start
        const auto bot_name =  bot.getApi().getMe()->username.c_str();
        std::cout << "Bot name: " << bot_name << std::endl;
        TgBot::TgLongPoll longPoll(bot);
        while (true)
        {
            std::cout << "Long poll..." << std::endl;
            longPoll.start();
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error! " << e.what() << '\n';
    }

    return 0;
}
