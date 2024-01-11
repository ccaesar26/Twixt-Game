#include "Game.h"
#include "GameException.h"

#include <fstream>
#include <regex>

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

	m_board->PlacePiece(pos, m_turn);
	NotifyPiecePlaced(pos);
}

void Game::CreateLink(const Position& pos1, const Position& pos2)
{
	if (m_state != EGameState::Playing)
	{
		throw InvalidStateException("Game is not playing");
	}

	m_board->LinkPieces(pos1, pos2);
	NotifyPiecesLinked(pos1, pos2);

	ILinkPtr link = m_board->GetLinkBetween(pos1, pos2);

	if (m_board->CheckIfWinningPlacement(link))
	{
		EColor winnerColor = link->GetColor();
		EGameResult gameResult = winnerColor == EColor::Black ? EGameResult::BlackWinner : EGameResult::RedWinner;
		NotifyGameOver(gameResult);
	}
}

void Game::RemoveLink(const Position& pos1, const Position& pos2)
{
	if (m_state != EGameState::Playing)
	{
		throw InvalidStateException("Game is not playing");
	}

	m_board->UnlinkPieces(pos1, pos2);
}

void Game::Reset()
{
	InitializeGame();
	NotifyGameRestarted();
}

void Game::Restore(const std::string& config)
{
	InitializeGame(config);
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

bool Game::IsFileValid(const std::string& fileName) const
{
	std::ifstream file(fileName);
	std::string config;

	if (!file.is_open())
	{
		return false;
	}
	
	std::getline(file, config);
	
	return RegexValidate(config);
}

bool Game::RegexValidate(const std::string& fileName) const
{
	std::regex regexPattern("^[01\\s]{256}[01][0123]$");
	if (std::regex_match(fileName, regexPattern)) {
		return true;
	}
	else {
		return false;
	}
}

void Game::SaveToFile(const std::string& fileName) const
{
	std::ofstream file(fileName);
	if (!file.is_open())
	{
		throw std::runtime_error("Failed to open file");
	}

	file << m_board->ToString();
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

void Game::NotifyPiecesLinked(const Position& pos1, const Position& pos2) const
{
	for (auto it = m_listeners.begin(); it != m_listeners.end(); ++it)
	{
		if (auto sp = it->lock()){
			sp->OnLinkPlaced(pos1, pos2);
		}
	}
}

//switch the current player pointer to the other player
void Game::SwitchTurn()
{
	m_turn = (m_turn == EColor::Black) ? EColor::Red : EColor::Black;
}

void Game::InitializeGame()
{
	m_board = IBoard::CreateBoard();
	m_turn = EColor::Red;
	m_state = EGameState::Playing;
	m_player1 = IPlayer::CreatePlayer(EColor::Red, "Player 1", m_board);
	m_player2 = IPlayer::CreatePlayer(EColor::Black, "Player 2", m_board);
}

void Game::InitializeGame(const std::string& config)
{
	std::string boardString;
	size_t pos = 0;
	while (config[pos] != '\n')
	{
		boardString += config[pos];
		pos++;
	}
	pos++;
	m_board = IBoard::CreateBoard(boardString);
	m_turn = static_cast<EColor>(config[pos] - '0');
	pos += 2;
	m_state = static_cast<EGameState>(config[pos] - '0');
	m_player1 = IPlayer::CreatePlayer(EColor::Red, "Player 1", m_board);
	m_player2 = IPlayer::CreatePlayer(EColor::Black, "Player 2", m_board);
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

EColor Game::GetCurrentPlayerColor() const
{
	return m_turn;
}

IPiecePtr Game::GetPiecePtr(const Position& pos) const
{
	return m_board->At(pos);
}
