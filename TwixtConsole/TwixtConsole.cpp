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

void TwixtConsole::printHelperMenu()
{
	std::cout << ">  Optiuni Disponibile <"<<std::endl;
	std::cout << "Tasta 1 -> Plasare Pod "<<std::endl;
	std::cout << "Tasta 2 -> Eliminare Pod " << std::endl;
	std::cout << "Tasta 3 -> Incarca Joc Din Fisier " << std::endl;
	std::cout << "Tasta 4 -> Salvare Joc In Fisier " << std::endl;
	std::cout << "Tasta 5 -> Solicita Remiza " << std::endl;
	std::cout << "Tasta 6 -> Termina Tura " << std::endl;
}

void TwixtConsole::OnPiecePlaced(const Position& pos)
{
	printBoard();
	std::cout << "Piesa a fost pusa cu succes!"<<std::endl;
	printHelperMenu();

}

void TwixtConsole::OnGameOver(const EGameResult& result)
{
}

void TwixtConsole::OnGameRestarted()
{
}

void TwixtConsole::printBoard()
{
}
