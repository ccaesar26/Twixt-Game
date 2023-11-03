#include "Game.h"

IGamePtr IGame::CreateGame()
{
	return std::make_shared<Game>();
}

void Game::PlacePiece(const Position& pos) const
{
	if (m_state != EGameState::Playing)
	{
		throw GameException("Game is not playing");
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
