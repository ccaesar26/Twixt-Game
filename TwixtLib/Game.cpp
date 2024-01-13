#include "Game.h"
#include "GameException.h"

#include <fstream>
#include <regex>

#include "ConverterLibrary/framework.h"

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

	//check if the player has a piece left
	if (m_turn == m_player1->GetColor() && m_player1->GetLimitPegs() < 1)
	{
		throw InvalidStateException("Player 1 has no pieces left");
	}
	else if (m_turn == m_player2->GetColor() && m_player2->GetLimitPegs() < 1)
	{
		throw InvalidStateException("Player 2 has no pieces left");
	}

	m_board->PlacePiece(pos, m_turn);

	const auto& peg = m_board->At(pos);

	if (m_turn == m_player1->GetColor())
	{
		m_player1->AddPeg(peg);
	}
	else
	{
		m_player2->AddPeg(peg);
	}
	NotifyPiecePlaced(pos);
}

void Game::CreateLink(const Position& pos1, const Position& pos2)
{
	if (m_state != EGameState::Playing)
	{
		throw InvalidStateException("Game is not playing");
	}

	//check if the player has available links
	if (m_turn == m_player1->GetColor() && m_player1->GetLimitLinks() < 1)
	{
		throw GameException("Player 1 has no links left");
	}
	else if (m_turn == m_player2->GetColor() && m_player2->GetLimitLinks() < 1)	{
		throw GameException("Player 2 has no links left");
	}

	//check if either piece is a null piece
	if (m_board->At(pos1) == nullptr || m_board->At(pos2) == nullptr)
	{
		throw GameException("Either one of the pieces is null");
	}

	//check if the player's color coincides with the pieces at pos1 and pos2
	if(m_board->At(pos1)->GetColor() != m_turn || m_board->At(pos2)->GetColor() != m_turn)
	{
		throw GameException("Player's color does not coincide with the pieces at pos1 and pos2");
	}

	m_board->LinkPieces(pos1, pos2);
	NotifyPiecesLinked(pos1, pos2);

	const ILinkPtr link = m_board->GetLinkBetween(pos1, pos2);
	
	if (m_turn == m_player1->GetColor())
	{
		m_player1->AddLink(link);
	}
	else
	{
		m_player2->AddLink(link);
	}

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


	const ILinkPtr link = m_board->GetLinkBetween(pos1, pos2);

	if (m_turn == m_player1->GetColor())
	{
		m_player1->RemoveLink(link);
	}
	else
	{
		m_player2->RemoveLink(link);
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
	return false;
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

void Game::Restore(const GameConfig& config)
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
	const GameConfig gameConfig(fileName);

	InitializeGame(gameConfig);
	NotifyGameRestarted();
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

void Game::RequestDraw(EColor currentPlayer) const
{
	if (m_state != EGameState::Playing)
	{
		throw InvalidStateException("Game is not playing");
	}

	NotifyGameOver(EGameResult::Draw);
}

void Game::NotifyDrawRequested() const
{
	for (auto it = m_listeners.begin(); it != m_listeners.end(); ++it)
	{
		if (const auto& sp = it->lock())
		{
			sp->OnDrawRequested(m_turn);
		}
	}
}

void Game::ReconfigureGame(const int boardSize,const int maxPegs,const int maxLinks)
{
	InitializeGame(boardSize, maxPegs, maxLinks);
}

void Game::SaveToFile(const std::string& fileName) const
{
	std::ofstream file(fileName);
	if (!file.is_open())
	{
		throw std::runtime_error("Failed to open file");
	}

	file << m_board->GetSize() << "\n";
	file << m_board->ToString() << "\n";
	file << m_board->LinksToString(EColor::Red) << "\n";
	file << m_board->LinksToString(EColor::Black) << "\n";
	file << static_cast<int>(m_turn) << "\n";
	file << static_cast<int>(m_state) << "\n";
	file << m_player1->GetLimitPegs() << "\n";
	file << m_player1->GetLimitLinks() << "\n";
}

void Game::NotifyBoardChanged(int newSize, int newMaxPegs, int newMaxLinks) const
{
	for (auto it = m_listeners.begin(); it != m_listeners.end(); ++it)
	{
		if (const auto& sp = it->lock())
		{
			sp->OnBoardChanged(newSize, newMaxPegs, newMaxLinks);
		}
	}
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
	m_board = IBoard::CreateBoard(24);
	m_turn = EColor::Red;
	m_state = EGameState::Playing;
	m_player1 = IPlayer::CreatePlayer(EColor::Red, "Player 1", m_board, 50, 50);
	m_player2 = IPlayer::CreatePlayer(EColor::Black, "Player 2", m_board, 50, 50);
}

void Game::InitializeGame(int boardSize, int maxPegs, int maxLinks)
{
	m_board = IBoard::CreateBoard(boardSize);
	m_turn = EColor::Red;
	m_state = EGameState::Playing;
	m_player1 = IPlayer::CreatePlayer(EColor::Red, "Player 1", m_board, maxPegs, maxLinks);
	m_player2 = IPlayer::CreatePlayer(EColor::Black, "Player 2", m_board, maxPegs, maxLinks);
}

void Game::InitializeGame(const GameConfig& config)
{
	m_board = IBoard::CreateBoard(config.GetBoardString(), config.GetPlayerOneLinks(), config.GetPlayerTwoLinks(), config.GetBoardSize());
	m_turn = static_cast<EColor>(config.GetTurn()[0] - '0');
	m_state = static_cast<EGameState>(config.GetState()[0] - '0');
	m_player1 = IPlayer::CreatePlayer(EColor::Red, "Player 1", m_board, config.GetPlayerPegLimit(), config.GetPlayerLinkLimit());
	m_player2 = IPlayer::CreatePlayer(EColor::Black, "Player 2", m_board, config.GetPlayerPegLimit(), config.GetPlayerLinkLimit());
	std::vector<IPiecePtr> pieces = m_board->GetPieces();
	std::vector<ILinkPtr> links = m_board->GetLinks();
	for (auto it = pieces.begin(); it != pieces.end(); ++it)
	{
		if ((*it)->GetColor() == m_player1->GetColor())
		{
			m_player1->AddPeg(*it);
		}
		else
		{
			m_player2->AddPeg(*it);
		}
	}
	for (auto it = links.begin(); it != links.end(); ++it)
	{
		if ((*it)->GetColor() == m_player1->GetColor())
		{
			m_player1->AddLink(*it);
		}
		else
		{
			m_player2->AddLink(*it);
		}
	}
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

int Game::GetPegsLimitNumber(EColor playerColor)
{
	if (playerColor == m_player1->GetColor())
	{
		return m_player1->GetLimitPegs();
	}
	else
	{
		return m_player2->GetLimitPegs();
	}
}

int Game::GetLinksLimitNumber(EColor playerColor)
{
	if (playerColor == m_player1->GetColor())
	{
		return m_player1->GetLimitLinks();
	}
	else
	{
		return m_player2->GetLimitLinks();
	}
}

int Game::GetAvailableLinksNumber(EColor playerColor) const
{
	if (playerColor == m_player1->GetColor())
	{
		return m_player1->GetAvailableLinks();
	}
	else
	{
		return m_player2->GetAvailableLinks();
	}
}

int Game::GetAvailablePegsNumber(EColor playerColor) const
{
	if (playerColor == m_player1->GetColor())
	{
		return m_player1->GetAvailablePegs();
	}
	else
	{
		return m_player2->GetAvailablePegs();
	}
}

int Game::GetBoardSize() const
{
	return m_board->GetSize();
}