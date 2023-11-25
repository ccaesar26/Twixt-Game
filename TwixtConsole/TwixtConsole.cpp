#include "TwixtConsole.h"

TwixtConsole::TwixtConsole()
{
}

void TwixtConsole::setGame(IGamePtr game)
{
	m_game = std::move(game);
}
