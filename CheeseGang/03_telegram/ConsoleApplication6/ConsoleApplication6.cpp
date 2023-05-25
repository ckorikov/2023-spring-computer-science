#include <iostream>
#include <tgbot/tgbot.h>
#include <vector>

int main() {
	TgBot::Bot bot("5892308271:AAEReyRjeuSpB2Yjf2yedZsF2DXLfeGJILc");

	std::vector<std::string> jokes = {
		"Why don't scientists trust atoms? Because they make up everything.",
		"I'm reading a book about anti-gravity. It's impossible to put down!",
		"Why don't programmers like nature? It has too many bugs.",
		"What do you call fake spaghetti? An impasta.",
		"Why was the math book sad? Because it had too many problems."
	};

	bot.getEvents().onCommand("joke", [&bot, &jokes](TgBot::Message::Ptr message) {
		// Generate a random joke
		int index = rand() % jokes.size();
		std::string joke = jokes[index];

		// Send the joke to the user
		bot.getApi().sendMessage(message->chat->id, joke);
	});

	bot.getApi().deleteWebhook();
	TgBot::TgLongPoll longPoll(bot);
	while (true) {
		try {
			longPoll.start();
		}
		catch (std::exception& e) {
			std::cerr << "Error: " << e.what() << std::endl;
		}
	}

	return 0;
}
