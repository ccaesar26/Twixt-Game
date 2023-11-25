#include "TwixtConsole.h"

TwixtConsole::TwixtConsole()
{
}

void TwixtConsole::setGame(IGamePtr game)
{
	m_game = std::move(game);
}

void TwixtConsole::printCurrentPlayer()
{
	std::cout << "Current player is: ";
	switch (m_game->GetCurrentPlayer())
	{
	case EColor::Black:
		std::cout << "Black";
		break;
	case EColor::Red:
		std::cout << "Red";
		break;
	}
	std::cout<<" player"<<std::endl;
}
