#include "TwixtConsole.h"
#include<utility>


int main()
{
	std::shared_ptr console = std::make_shared<TwixtConsole>();
	std::shared_ptr gameLogic = IGame::CreateGame();

	console->setGame(gameLogic);
	return 0;
}