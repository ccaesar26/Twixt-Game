#include "Game.h"

IGamePtr IGame::CreateGame()
{
	return std::make_shared<Game>();
}

void Game::PlacePiece(const Position& pos)
{
	if (m_state != EGameState::Playing)
	{
		throw GameException("Game is not playing");
	}

	m_board.PlacePiece(pos, m_turn);
	NotifyPiecePlaced();

	if (m_board.CheckIfWinningPlacement(pos, m_turn))
	{
		//if won by black
		if (m_turn == EColor::Black)
		{
			NotifyGameOver(EGameResult::BlackWinner);
		}
		else if (m_turn == EColor::Red)
		{
			NotifyGameOver(EGameResult::RedWinner);
		}
	}
	else if (m_state != EGameState::Draw)
	{
		SwitchTurn();
	}
}

bool Game::IsDraw() const
{
	return m_state == EGameState::Draw;
}

bool Game::IsWon() const
{
	return m_state == EGameState::WonByBlack || m_state == EGameState::WonByRed;
}

bool Game::IsGameOver() const
{
	return IsDraw() || IsWon();
}
