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
	switch (m_game->GetCurrentPlayerColor())
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
	std::cout << "Tasta 0 -> Plasare Piesa " << std::endl;
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
	printBoard();
	switch (result)
	{
	case EGameResult::BlackWinner:
		std::cout << "The black player is the winner!!\n";
		break;
	case EGameResult::RedWinner:
			std::cout << "The red player is the winner!!\n";
			break;
	case EGameResult::Draw:
			std::cout << "The game ended in a draw!!\n";
			break;
	}

}

void TwixtConsole::OnGameRestarted()
{
	printBoard();
	std::cout << "The game was restarted!!\n";
	printHelperMenu();
}

void TwixtConsole::printBoard()
{
}

void TwixtConsole::PlacePiece(Position& pos)
{
	m_game->PlacePiece(pos);
	OnPiecePlaced(pos);
}

void TwixtConsole::PlaceBridge(Position& pos1, Position& pos2)
{
}

void TwixtConsole::DeleteBridge(Position& pos1, Position& pos2)
{
}

void TwixtConsole::EndTurn()
{
}
