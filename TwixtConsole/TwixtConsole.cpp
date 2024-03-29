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
	std::cout << "Tasta 6 -> Termina Tura " << std::endl<<std::endl;
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

void TwixtConsole::OnLinkPlaced(const Position& pos1, const Position& pos2)
{
}

void TwixtConsole::printBoard()
{
	for (size_t i = 0; i < 24; i++)
	{
		if (i == 1 || i == 23)
			std::cout << "--|---------------------------------------------|--"<<std::endl;
		for (size_t j = 0; j < 23; j++)
		{
			
			
			const auto piecePtr = m_game->GetPiecePtr(Position(i, j));
			if (piecePtr)
			{
				if(piecePtr->GetColor() == EColor::Black)
					std::cout << " B";
				else
					std::cout << " R";
			}
			else
			{
				std::cout << " O";
			}
			if (j == 0||j==23)
				std::cout << "|";
		}
		std::cout << " |";
		const auto piecePtr = m_game->GetPiecePtr(Position(i, 23));
		if (piecePtr)
		{
			if (piecePtr->GetColor() == EColor::Black)
				std::cout << " B";
			else
				std::cout << " R";
		}
		else
		{
			std::cout << " O";
		}
		
		std::cout <<"\n";
	}
	std::cout << "\n";
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
	m_game->SwitchTurn();
}

void TwixtConsole::SaveToFile()
{
	m_game->SaveToFile("Game safe");
	std::cout << "Salvarea a fost efectuata cu succes!"<<"\n\n";
}

void TwixtConsole::LoadFromFile()
{
	m_game->LoadFromFile("Game safe");
	std::cout << "Salvarea a fost efectuata cu succes!" << "\n\n";
}

void TwixtConsole::SolicitDraw()
{
	std::cout << "Ambii jucatori vor remiza?\n"<<"Da/Nu\n";
	std::string s;
	std::cin >> s;
	if (s == "DA" || s == "da" || s == "dA" || s == "Da")
		m_game->IsDraw();
	else
		std::cout << "Celalalt jucator nu a vrut sa fie remiza!" << "\n\n";
}

