#include "Game.h"
#include "GameException.h"

IGamePtr IGame::CreateGame()
{
	return std::make_shared<Game>();
}

void Game::PlacePiece(const Position& pos)
{
	if (m_state != EGameState::Playing)
	{
		throw InvalidStateException("Game is not playing");
	}

	m_board.PlacePiece(pos, m_turn);
	NotifyPiecePlaced(pos);

	if (m_board.CheckIfWinningPlacement(pos, m_turn))
	{
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

void Game::NotifyGameOver(EGameResult gameResult) const
{
	for (auto it = m_listeners.begin(); it != m_listeners.end(); ++it)
	{
		if (const auto sp = it->lock())
		{
			sp->OnGameOver(gameResult);
		}
	}
}

void Game::NotifyGameRestarted() const
{
	for (auto it = m_listeners.begin(); it != m_listeners.end(); ++it)
	{
		if (const auto sp = it->lock())
		{
			sp->OnGameRestarted();
		}
	}
}

void Game::NotifyPiecePlaced(const Position& pos) const
{
	for (auto it = m_listeners.begin(); it != m_listeners.end(); ++it)
	{
		if (auto sp = it->lock())
		{
			sp->OnPiecePlaced(pos);
		}
	}
}

void Game::RemoveListener(const IGameListenerPtr listener)
{
	m_listeners.push_back(listener);
}

void Game::SwitchTurn()
{
	m_turn = m_turn == EColor::Black ? EColor::Red : EColor::Black;
}

void Game::InitializeGame()
{
	m_board = Board();
	m_turn = EColor::Black;
	m_state = EGameState::Playing;
}

Game::Game()
{
	InitializeGame();
}

void Game::AddListener(IGameListener* listener)
{
	auto f = [listener](const IGameListenerWeakPtr& weak)
	{
		const auto sp = weak.lock();
		return !sp || sp.get() == listener;
	};

	//m_listeners.erase(std::remove_if(m_listeners.begin(), m_listeners.end(), f));
	std::erase_if(m_listeners, f);
}
