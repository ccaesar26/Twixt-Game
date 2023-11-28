#include "Game.h"
#include "GameException.h"

#include <fstream>

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

void Game::Reset()
{
}

void Game::Restore(const std::string& config) const
{
}

bool Game::IsDraw() const
{
	return m_state == EGameState::Draw;
}

bool Game::IsWon() const
{
	return m_state == EGameState::WonByBlack || m_state == EGameState::WonByRed;
}

void Game::LoadFromFile(const std::string& fileName)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		throw GameException("Could not open file");
	}

	std::string config;
	std::getline(file, config);
	InitializeGame(config);
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

void Game::SaveToFile(const std::string& fileName) const
{
	std::ofstream file(fileName);
	if (!file.is_open())
	{
		throw std::runtime_error("Failed to open file");
	}

	file << m_board.ToString();
	file << static_cast<int>(m_turn) << "\n";
	file << static_cast<int>(m_state) << "\n";
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

void Game::InitializeGame(const std::string& config, EColor turn)
{
	std::string boardString;
	size_t pos = 0;
	while (config[pos] != '\n')
	{
		boardString += config[pos];
		pos++;
	}
	pos++;
	m_board = Board(boardString);
	m_turn = static_cast<EColor>(config[pos] - '0');
	pos += 2;
	m_state = static_cast<EGameState>(config[pos] - '0');
}

Game::Game()
{
	InitializeGame();
}

void Game::AddListener(IGameListenerPtr listener)
{
	m_listeners.push_back(listener);
}

void Game::RemoveListener(IGameListener* listener)
{
	auto f = [listener](const IGameListenerWeakPtr& weak)
	{
		const auto sp = weak.lock();
		return !sp || sp.get() == listener;
	};

	//m_listeners.erase(std::remove_if(m_listeners.begin(), m_listeners.end(), f));
	std::erase_if(m_listeners, f);
}

EColor Game::GetCurrentPlayer() const
{
	return m_turn;
}

IPiecePtr Game::GetPiecePtr(const Position& pos) const
{
	return m_board.At(pos);
}
