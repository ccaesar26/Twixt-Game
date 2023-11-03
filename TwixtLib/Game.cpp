#include "Game.h"

IGamePtr IGame::CreateGame()
{
	return std::make_shared<Game>();
}
