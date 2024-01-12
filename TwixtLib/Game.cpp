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

	const ILinkPtr link = m_board->GetLinkBetween(pos1, pos2);

	if (m_board->CheckIfWinningPlacement(link))
	{
		const EColor winnerColor = link->GetColor();
		const EGameResult gameResult = winnerColor == EColor::Black ? EGameResult::BlackWinner : EGameResult::RedWinner;
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
	NotifyLinkRemoved(pos1, pos2);
}

void IdentifyChainsDFS(std::vector<std::vector<Position>>& chains)
{
	std::vector<Position> visited;

	// Iterate through all the pieces on the board.
	
}

void EvaluateAndSelectBestChain(const std::vector<std::vector<Position>>& chains, std::vector<Position>& bestChain)
{
	// Evaluate each chain based on length and proximity to edges.
	// Select the chain with the best combination of criteria.
	// Update the 'bestChain' vector with the selected chain.
}

void IdentifyBestMoveForChain(const std::vector<Position>& bestChain, Position& bestMove)
{
	// Identify potential moves that bring the chain closer to the edges.
	// Update the 'bestMove' with the selected move.
}

bool IsValidMove(const Position& move)
{
	// Check if the move is a legal move and linking the new peg won't intersect with other links.
	// Return true if the move is valid, false otherwise.
}

std::pair<Position, std::vector<std::pair<Position, Position>>> Game::Recommend()
{
	// 1. DFS to Identify Chains
	std::vector<std::vector<Position>> chains;
	IdentifyChainsDFS(chains);

	// 2. Evaluate Chains
	// 3. Select Best Chain
	std::vector<Position> bestChain;
	EvaluateAndSelectBestChain(chains, bestChain);

	// 4. Identify Best Move
	Position bestMove;
	IdentifyBestMoveForChain(bestChain, bestMove);

	// 5. Check Validity of Moves
	if (IsValidMove(bestMove))
	{
		// 6. Recommend Best Move
		return std::make_pair(bestMove, std::vector<std::pair<Position, Position>>());
	}
	else
	{
		// Handle the case when no valid moves are found.
		// You might want to explore other options or return a special value.
		return std::make_pair(Position(-1, -1), std::vector<std::pair<Position, Position>>());
	}
}

void Game::Reset()
{
	InitializeGame();
	NotifyGameRestarted();
}

void Game::Restore(const std::string& boardString, const std::string& turn, const std::string& state)
{
	InitializeGame(boardString, turn, state);
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

	std::string boardString;
	std::string turn;
	std::string state;
	std::getline(file, boardString);
	std::getline(file, turn);
	std::getline(file, state);
	InitializeGame(boardString, turn, state);
}

bool Game::IsGameOver() const
{
	return IsDraw() || IsWon();
}

void Game::NotifyGameOver(EGameResult gameResult) const
{
	for (auto it = m_listeners.begin(); it != m_listeners.end(); ++it)
	{
		if (const auto& sp = it->lock())
		{
			sp->OnGameOver(gameResult);
		}
	}
}

void Game::NotifyGameRestarted() const
{
	for (auto it = m_listeners.begin(); it != m_listeners.end(); ++it)
	{
		if (const auto& sp = it->lock())
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
	file << "\n";
	file << static_cast<int>(m_turn) << "\n";
	file << static_cast<int>(m_state) << "\n";
}

void Game::NotifyPiecePlaced(const Position& pos) const
{
	for (auto it = m_listeners.begin(); it != m_listeners.end(); ++it)
	{
		if (const auto& sp = it->lock())
		{
			sp->OnPiecePlaced(pos);
		}
	}
}

void Game::NotifyPiecesLinked(const Position& pos1, const Position& pos2) const
{
	for (auto it = m_listeners.begin(); it != m_listeners.end(); ++it)
	{
		if (const auto& sp = it->lock()){
			sp->OnLinkPlaced(pos1, pos2);
		}
	}
}

void Game::NotifyLinkRemoved(const Position& pos1, const Position& pos2) const
{
	for (auto it = m_listeners.begin(); it != m_listeners.end(); ++it)
	{
		if (const auto& sp = it->lock())
		{
			sp->OnLinkRemoved(pos1, pos2);
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

void Game::InitializeGame(const std::string& boardString, const std::string& turn, const std::string& state)
{
	m_board = IBoard::CreateBoard(boardString);
	m_turn = static_cast<EColor>(turn[0] - '0');
	m_state = static_cast<EGameState>(state[0] - '0');
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
		const auto& sp = weak.lock();
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
