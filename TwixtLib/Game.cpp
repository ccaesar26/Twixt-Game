#include "Game.h"

void Game::PlacePiece(const Position& pos) const
{
	if (m_state != EGameState::Playing)
	{
		throw GameException("Game is not playing");
	}
}