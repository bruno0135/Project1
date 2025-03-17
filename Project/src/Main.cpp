#include <iostream>
#include <raylib.h>
#include "Game.h"
#include "MemoryLeaks.h"
#include "Globals.h"
#include "LogMessages.h"

int main()
{

	Game *game;
	AppStatus status;
	int main_return = EXIT_SUCCESS;

	LOG("Application start");
	game = new Game();
	status = game->Initialise(GAME_SCALE_FACTOR);
	if (status != AppStatus::OK)
	{
		LOG("Failed to initialise game");
		main_return = EXIT_FAILURE;
	}

	while (status == AppStatus::OK)
	{

		status = game->Update();
		if (status != AppStatus::OK)
		{
			if (status == AppStatus::ERROR)      main_return = EXIT_FAILURE;
			else if (status == AppStatus::QUIT)  main_return = EXIT_SUCCESS;
			break;
		}
		game->Render();
	}

	LOG("Application finish");
	game->Cleanup();

	LOG("Bye :)");
	delete game;

	return main_return;
}

