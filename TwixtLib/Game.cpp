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
	if (m_turn == m_player2->GetColor() && m_player2->GetLimitPegs() < 1)
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
	if (m_turn == m_player2->GetColor() && m_player2->GetLimitLinks() < 1)
	{
		throw GameException("Player 2 has no links left");
	}

	//check if either piece is a null piece
	if (m_board->At(pos1) == nullptr || m_board->At(pos2) == nullptr)
	{
		throw GameException("Either one of the pieces is null");
	}

	//check if the player's color coincides with the pieces at pos1 and pos2
	if (m_board->At(pos1)->GetColor() != m_turn || m_board->At(pos2)->GetColor() != m_turn)
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

void Game::EvaluateAndSortChains(const std::set<std::vector<Position>>& chains, std::vector<std::vector<Position>>& sortedChains)
{
	// Custom comparator for sorting based on the least cumulative distance
	auto compareChains = [this](const std::vector<Position>& chain1, const std::vector<Position>& chain2) {
		int minDistanceChain1 = CalculateMinCumulativeDistance(chain1);
		int minDistanceChain2 = CalculateMinCumulativeDistance(chain2);
		return minDistanceChain1 < minDistanceChain2;
		};

	// Copy chains to a vector and sort them using the custom comparator
	sortedChains.assign(chains.begin(), chains.end());
	std::sort(sortedChains.begin(), sortedChains.end(), compareChains);
}

int Game::CalculateMinCumulativeDistance(const std::vector<Position>& chain)
{
	// Calculate and return the minimum cumulative distance for a given chain
	int minCummulatedDistance = INT_MAX;

	for (const auto& position : chain)
	{
		int distance;
		if (m_turn == EColor::Red)
		{
			const int topDifference = position.row;
			const int bottomDifference = m_board->GetSize() - position.row;
			distance = topDifference + bottomDifference;
		}
		else
		{
			const int leftDifference = position.col;
			const int rightDifference = m_board->GetSize() - position.col;
			distance = leftDifference + rightDifference;
		}

		if (distance < minCummulatedDistance)
		{
			minCummulatedDistance = distance;
		}
	}

	return minCummulatedDistance;
}

std::pair<std::vector<Position>, std::pair<Position, Position>> Game::FindImprovableChain(const std::vector<std::vector<Position>>& sortedChains)
{
	for (const auto& chain : sortedChains)
	{
		// Get all extreme pieces of the chain based on the current player's color
		std::vector<Position> extremePieces;
		GetExtremePieces(chain, extremePieces);

		// Check if any of the extreme pieces have potential neighbors that can be added to the chain
		for (const auto& extreme : extremePieces)
		{
			std::vector<Position> potentialNeighbors = m_board->GetPotentialNeighbours(extreme);

			for (const auto& potentialNeighbor : potentialNeighbors)
			{
				if (std::find(chain.begin(), chain.end(), potentialNeighbor) == chain.end())
				{
					// Potential neighbor is not already part of the chain, so it can be added
					return std::make_pair(chain, std::make_pair(extreme, potentialNeighbor)); // This chain can be improved
				}
			}
		}
	}

	// No improvable chain found
	return std::make_pair(std::vector<Position>(), std::make_pair(Position(), Position()));
}
void Game::GetExtremePieces(const std::vector<Position>& chain, std::vector<Position>& extremePieces)
{
	// Determine extreme pieces based on the current player's color
	if (m_turn == EColor::Red)
	{
		int minRow = INT_MAX;
		int maxRow = -1;

		for (const auto& position : chain)
		{
			if (position.row < minRow)
			{
				minRow = position.row;
			}

			if (position.row > maxRow)
			{
				maxRow = position.row;
			}
		}

		for (const auto& position : chain)
		{
			if (position.row == minRow || position.row == maxRow)
			{
				extremePieces.push_back(position);
			}
		}
	}
	else
	{
		int minCol = INT_MAX;
		int maxCol = -1;

		for (const auto& position : chain)
		{
			if (position.col < minCol)
			{
				minCol = position.col;
			}

			if (position.col > maxCol)
			{
				maxCol = position.col;
			}
		}

		for (const auto& position : chain)
		{
			if (position.col == minCol || position.col == maxCol)
			{
				extremePieces.push_back(position);
			}
		}
	}
}

std::pair<Position, Position> Game::Recommend()
{
	// 1. DFS to Identify Chains
	std::set<std::vector<Position>> chains = m_board->GetChains(m_turn);

	std::vector<std::vector<Position>> sortedChains;
	EvaluateAndSortChains(chains, sortedChains);

	std::vector<Position> improvingChain;
	std::pair<Position, Position> improvingLink;

	// 2. Identify Improvable Chains
	std::tie(improvingChain, improvingLink) = FindImprovableChain(sortedChains);

	return improvingLink;
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

	NotifyGameRestarted();

	InitializeGame(gameConfig);

	NotifyGameLoaded();
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

void Game::NotifyGameLoaded() const
{
	for (auto it = m_listeners.begin(); it != m_listeners.end(); ++it)
	{
		if (const auto& sp = it->lock())
		{
			sp->OnGameLoaded();
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
	if (std::regex_match(fileName, regexPattern))
	{
		return true;
	}
	return false;
}

void Game::EndInDraw() const
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

void Game::ReconfigureGame(const int boardSize, const int maxPegs, const int maxLinks)
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
		if (const auto& sp = it->lock())
		{
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
	m_board = IBoard::CreateBoard(config.GetBoardString(), config.GetPlayerOneLinks(), config.GetPlayerTwoLinks(),
	                              config.GetBoardSize(), [this](const Position pos1, const Position pos2, const EColor color)
	                              {
									  m_turn = color;
		                              NotifyPiecesLinked(pos1, pos2);
	                              });
	m_turn = static_cast<EColor>(config.GetTurn()[0] - '0');
	m_state = static_cast<EGameState>(config.GetState()[0] - '0');
	m_player1 = IPlayer::CreatePlayer(EColor::Red, "Player 1", m_board, config.GetPlayerPegLimit(),
	                                  config.GetPlayerLinkLimit());
	m_player2 = IPlayer::CreatePlayer(EColor::Black, "Player 2", m_board, config.GetPlayerPegLimit(),
	                                  config.GetPlayerLinkLimit());
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
	return m_player2->GetLimitPegs();
}

int Game::GetLinksLimitNumber(EColor playerColor)
{
	if (playerColor == m_player1->GetColor())
	{
		return m_player1->GetLimitLinks();
	}
	return m_player2->GetLimitLinks();
}

int Game::GetAvailableLinksNumber(EColor playerColor) const
{
	if (playerColor == m_player1->GetColor())
	{
		return m_player1->GetAvailableLinks();
	}
	return m_player2->GetAvailableLinks();
}

int Game::GetAvailablePegsNumber(EColor playerColor) const
{
	if (playerColor == m_player1->GetColor())
	{
		return m_player1->GetAvailablePegs();
	}
	return m_player2->GetAvailablePegs();
}

int Game::GetBoardSize() const
{
	return m_board->GetSize();
}
