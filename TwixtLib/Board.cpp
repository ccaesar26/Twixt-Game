#include "Board.h"

#include <functional>

#include "GameException.h"
#include <stack>
#include "Peg.h"
#include <set>

IBoardPtr IBoard::CreateBoard()
{
	return std::make_shared<Board>();
}

IBoardPtr IBoard::CreateBoard(int size)
{
	return std::make_shared<Board>(size);
}

IBoardPtr IBoard::CreateBoard(const std::string& config, const std::string& playerOneLinks,
                              const std::string& playerTwoLinks, int size, const std::
                              function<void(Position pos1, Position pos2, EColor color)>&
                              notificationCallback)
{
	return std::make_shared<Board>(config, playerOneLinks, playerTwoLinks, size, notificationCallback);
}

Board::Board(const int size) : m_size(size)
{
	m_board.resize(m_size);
	for (int i = 0; i < m_size; i++)
	{
		m_board[i].resize(m_size);
		for (int j = 0; j < m_size; j++)
		{
			m_board[i][j] = nullptr;
		}
	}
}

Board::Board(const std::string& boardString, const std::string& playerOneLinks, const std::string& playerTwoLinks,
             int size, const std::
             function<void(Position pos1, Position pos2, EColor color)>& notificationCallback)
{
	int pos = 0;
	m_board.resize(size);
	while (boardString[pos] != '\n')
	{
		if (boardString[pos] == ' ')
		{
			m_board[pos / size].push_back(nullptr);
		}
		else if (boardString[pos] == '0' || boardString[pos] == '1')
		{
			Position pegPos;
			pegPos.row = pos / size;
			pegPos.col = pos % size;
			m_board[pos / size].push_back(std::make_shared<Peg>(static_cast<EColor>(boardString[pos] - '0'), pegPos));
		}
		pos++;
	}
	m_size = size;
	//the links are read from the file in the format: "row1 col1 row2 col2 row3 col3 row4 col4 ... \n"
	//so we use a stringstream to read the links
	std::stringstream ss1(playerOneLinks);
	std::stringstream ss2(playerTwoLinks);

	int row1, col1, row2, col2;
	while (ss1 >> row1 >> col1 >> row2 >> col2)
	{
		LinkPieces(Position(row1, col1), Position(row2, col2));
		const auto color = m_board[row1][col1]->GetColor();
		notificationCallback(Position(row1, col1), Position(row2, col2), color);
	}
	while (ss2 >> row1 >> col1 >> row2 >> col2)
	{
		LinkPieces(Position(row1, col1), Position(row2, col2));
		const auto color = m_board[row1][col1]->GetColor();
		notificationCallback(Position(row1, col1), Position(row2, col2), color);
	}
}

Board::Board(const Board& other)
{
	m_size = other.m_size;
	m_board.resize(m_size);
	for (int i = 0; i < m_size; i++)
	{
		m_board[i].resize(m_size);
		for (int j = 0; j < m_size; j++)
		{
			if (other.m_board[i][j] == nullptr)
			{
				m_board[i][j] = nullptr;
			}
			else
			{
				Position pegPos;
				pegPos.row = i;
				pegPos.col = j;
				m_board[i][j] = std::make_shared<Peg>(other.m_board[i][j]->GetColor(), pegPos);
			}
		}
	}
}

Board& Board::operator=(const Board& other)
{
	if (this == &other)
	{
		return *this;
	}
	m_size = other.m_size;
	m_board.resize(m_size);
	for (int i = 0; i < m_size; i++)
	{
		m_board[i].resize(m_size);
		for (int j = 0; j < m_size; j++)
		{
			if (other.m_board[i][j] == nullptr)
			{
				m_board[i][j] = nullptr;
			}
			else
			{
				Position pegPos;
				pegPos.row = i;
				pegPos.col = j;
				m_board[i][j] = std::make_shared<Peg>(other.m_board[i][j]->GetColor(), pegPos);
			}
		}
	}
	return *this;
}

Board& Board::operator=(Board&& other) noexcept
{
	if (this == &other)
	{
		return *this;
	}
	m_size = other.m_size;
	m_board = std::move(other.m_board);
	return *this;
}

int Board::GetSize() const
{
	return m_size;
}

std::string Board::ToString() const
{
	std::string result;
	for (int i = 0; i < m_size; i++)
	{
		for (int j = 0; j < m_size; j++)
		{
			if (m_board[i][j] == nullptr)
			{
				result += " ";
			}
			else if (m_board[i][j]->GetColor() == EColor::Black)
			{
				result += static_cast<int>(EColor::Black) + '0';
			}
			else if (m_board[i][j]->GetColor() == EColor::Red)
			{
				result += static_cast<int>(EColor::Red) + '0';
			}
		}
	}

	return result;
}

std::string Board::LinksToString(EColor playerColor) const
{
	//return a string containing all the links of a player on the board
	//the string will be in the format: "row1 col1 row2 col2 row3 col3 row4 col4 ... \n"

	std::string result;
	for (const auto& link : m_links)
	{
		if (link->GetColor() == playerColor)
		{
			result += std::to_string(link->GetPiece1()->GetPosition().row) + " " +
				std::to_string(link->GetPiece1()->GetPosition().col) + " " +
				std::to_string(link->GetPiece2()->GetPosition().row) + " " + std::to_string(
					link->GetPiece2()->GetPosition().col) + " ";
		}
	}

	return result;
}

Board::Board(Board&& other) noexcept
{
	m_size = other.m_size;
	m_board = std::move(other.m_board);
}

void Board::PlacePiece(const Position pos, const EColor color)
{
	if (pos.row < 0 || pos.row >= m_size || pos.col < 0 || pos.col >= m_size)
	{
		throw GameException("Invalid position");
	}
	if (m_board[pos.row][pos.col] != nullptr)
	{
		throw GameException("Position is already occupied");
	}

	if ((pos.row == 0 && pos.col == 0) || (pos.row == 0 && pos.col == m_size - 1) || (pos.row == m_size - 1 && pos.col
		== 0) || (pos.row == m_size - 1 && pos.col == m_size - 1))
	{
		throw GameException("Invalid position");
	}

	if ((pos.row == 0 || pos.row == m_size - 1) && color == EColor::Black)
	{
		throw GameException("Invalid position");
	}

	if ((pos.col == 0 || pos.col == m_size - 1) && color == EColor::Red)
	{
		throw GameException("Invalid position");
	}

	m_board[pos.row][pos.col] = IPiece::Produce(color, pos);
}

bool DoSegmentsIntersect(const Position& p1, const Position& p2, const Position& p3, const Position& p4)
{
	const int d1 = (p3.col - p4.col) * (p1.row - p3.row) + (p3.row - p4.row) * (p3.col - p1.col);
	const int d2 = (p3.col - p4.col) * (p2.row - p3.row) + (p3.row - p4.row) * (p3.col - p2.col);
	const int d3 = (p1.col - p2.col) * (p3.row - p1.row) + (p1.row - p2.row) * (p1.col - p3.col);
	const int d4 = (p1.col - p2.col) * (p4.row - p1.row) + (p1.row - p2.row) * (p1.col - p4.col);
	return d1 * d2 < 0 && d3 * d4 < 0;
}

std::vector<Position> Board::GetPotentialNeighbours(const Position& pos)
{
	std::vector<Position> result;
	//we check all the positions that are a chess knight move away from the given position
	result.emplace_back(pos.row - 2, pos.col - 1);
	result.emplace_back(pos.row - 2, pos.col + 1);
	result.emplace_back(pos.row - 1, pos.col - 2);
	result.emplace_back(pos.row - 1, pos.col + 2);
	result.emplace_back(pos.row + 1, pos.col - 2);
	result.emplace_back(pos.row + 1, pos.col + 2);
	result.emplace_back(pos.row + 2, pos.col - 1);
	result.emplace_back(pos.row + 2, pos.col + 1);

	//get the color of the piece at the given position
	const EColor color = m_board[pos.row][pos.col]->GetColor();

	//based on the color of the piece, we sort the positions in the result vector
	//if the piece is black, we sort the positions by the criteria that they are closer to the sides of the board (left and right)
	if (color == EColor::Black)
	{
		std::ranges::sort(result, [this](const Position& pos1, const Position& pos2)
		{
			return abs(pos1.col - m_size / 2) > abs(pos2.col - m_size / 2);
		});
	}
	if (color == EColor::Red)
	{
		//if the piece is red, we sort the positions by the criteria that they are closer to the top and bottom of the board
		std::ranges::sort(result, [this](const Position& pos1, const Position& pos2)
		{
			return abs(pos1.row - m_size / 2) > abs(pos2.row - m_size / 2);
		});
	}

	//now we remove the positions that are outside the board
	result.erase(std::ranges::remove_if(result, [this](const Position& pos) { return !IsPositionValid(pos); }).begin(),
	             result.end());

	//we remove the positions that are already occupied
	result.erase(std::ranges::remove_if(result, [this](const Position& pos)
	{
		return m_board[pos.row][pos.col] != nullptr;
	}).begin(), result.end());

	//we remove the positions that are blocked by a link
	std::erase_if(result,
	              [this, pos](const Position& pos2)
	              {
		              return std::ranges::any_of(m_links,
		                                         [this, pos, pos2](const ILinkPtr& link)
		                                         {
			                                         return DoSegmentsIntersect(pos, pos2,
				                                         link->GetPiece1()->GetPosition(),
				                                         link->GetPiece2()->GetPosition());
		                                         });
	              });

	return result;
}

void Board::LinkPieces(Position pos1, Position pos2)
{
	if (pos1.row < 0 || pos1.row >= m_size || pos1.col < 0 || pos1.col >= m_size)
	{
		throw GameException("Invalid position");
	}
	if (pos2.row < 0 || pos2.row >= m_size || pos2.col < 0 || pos2.col >= m_size)
	{
		throw GameException("Invalid position");
	}
	if (m_board[pos1.row][pos1.col] == nullptr || m_board[pos2.row][pos2.col] == nullptr)
	{
		throw GameException("Invalid position");
	}

	//check if the two positions are adjacent (a chess knight move away)
	if (abs(pos1.row - pos2.row) + abs(pos1.col - pos2.col) != 3)
	{
		throw GameException("Pieces are not adjacent");
	}

	//check if the two positions have pieces of a different color
	if (m_board[pos1.row][pos1.col]->GetColor() != m_board[pos2.row][pos2.col]->GetColor())
	{
		throw GameException("Differently colored pieces");
	}

	//check if the two positions are blocked by another link in the links vector
	for (const auto& link : m_links)
	{
		if (DoSegmentsIntersect(pos1, pos2, link->GetPiece1()->GetPosition(), link->GetPiece2()->GetPosition()))
		{
			throw GameException("Overlapping link");
		}
	}

	const EColor color = m_board[pos1.row][pos1.col]->GetColor();

	const ILinkPtr link = ILink::Produce(m_board[pos1.row][pos1.col], m_board[pos2.row][pos2.col], color);

	AddLink(link);

	m_board[pos1.row][pos1.col]->AddNeighbor(m_board[pos2.row][pos2.col]);
	m_board[pos2.row][pos2.col]->AddNeighbor(m_board[pos1.row][pos1.col]);
}

void Board::UnlinkPieces(Position pos1, Position pos2)
{
	RemoveLink(GetLinkBetween(pos1, pos2));

	m_board[pos1.row][pos1.col]->RemoveNeighbor(m_board[pos2.row][pos2.col]);
	m_board[pos2.row][pos2.col]->RemoveNeighbor(m_board[pos1.row][pos1.col]);
}


IPiecePtr Board::At(const Position pos) const
{
	if (pos.row < 0 || pos.row >= m_size || pos.col < 0 || pos.col >= m_size)
	{
		throw GameException("Invalid position");
	}
	return m_board[pos.row][pos.col];
}

bool Board::IsPositionValid(const Position& pos) const
{
	return pos.row >= 0 && pos.row < m_size && pos.col >= 0 && pos.col < m_size;
}

bool Board::CheckPathToRows(const Position& pos, int targetUpperRow, int targetLowerRow) const
{
	if (!IsPositionValid(pos))
	{
		return false;
	}

	std::stack<Position> stack;

	std::set<Position> visited;

	stack.push(pos);

	bool foundUpperRow = false;
	bool foundLowerRow = false;

	while (!stack.empty() && !(foundUpperRow && foundLowerRow))
	{
		Position currentPos = stack.top();
		stack.pop();

		if (!IsPositionValid(currentPos) || visited.contains(currentPos) == true)
		{
			continue;
		}

		visited.insert(currentPos);

		const IPiecePtr currentPiece = At(currentPos);

		if (currentPiece && currentPiece->GetPosition().row == targetUpperRow)
		{
			foundUpperRow = true;
		}

		if (currentPiece && currentPiece->GetPosition().row == targetLowerRow)
		{
			foundLowerRow = true;
		}

		for (const auto& neighbor : currentPiece->GetNeighbors())
		{
			if (!visited.contains(neighbor->GetPosition()))
			{
				stack.push(neighbor->GetPosition());
			}
		}
	}

	return foundUpperRow && foundLowerRow;
}

bool Board::CheckPathToCols(const Position& pos, int targetLeftCol, int targetRightCol) const
{
	if (!IsPositionValid(pos))
	{
		return false;
	}

	std::stack<Position> stack;

	std::set<Position> visited;

	stack.push(pos);

	bool foundLeftCol = false;
	bool foundRightCol = false;

	while (!stack.empty() && !(foundLeftCol && foundRightCol))
	{
		Position currentPos = stack.top();
		stack.pop();

		if (!IsPositionValid(currentPos) || visited.contains(currentPos) == true)
		{
			continue;
		}

		visited.insert(currentPos);

		const IPiecePtr currentPiece = At(currentPos);

		if (currentPiece && currentPiece->GetPosition().col == targetLeftCol)
		{
			foundLeftCol = true;
		}

		if (currentPiece && currentPiece->GetPosition().col == targetRightCol)
		{
			foundRightCol = true;
		}

		for (const auto& neighbor : currentPiece->GetNeighbors())
		{
			if (!visited.contains(neighbor->GetPosition()))
			{
				stack.push(neighbor->GetPosition());
			}
		}
	}

	return foundLeftCol && foundRightCol;
}

bool Board::CheckPath(const Position& pos, int targetStart, int targetEnd, EColor playerColor) const
{
	if (!IsPositionValid(pos))
	{
		return false;
	}

	std::stack<Position> stack;
	std::set<Position> visited;

	stack.push(pos);

	bool foundStart = false;
	bool foundEnd = false;

	while (!stack.empty() && !(foundStart && foundEnd))
	{
		Position currentPos = stack.top();
		stack.pop();

		if (!IsPositionValid(currentPos) || visited.contains(currentPos) == true)
		{
			continue;
		}

		visited.insert(currentPos);

		const IPiecePtr currentPiece = At(currentPos);

		if (playerColor == EColor::Red)
		{
			if (currentPiece && currentPiece->GetPosition().row == targetStart)
			{
				foundStart = true;
			}

			if (currentPiece && currentPiece->GetPosition().row == targetEnd)
			{
				foundEnd = true;
			}
		}
		else if (playerColor == EColor::Black)
		{
			if (currentPiece && currentPiece->GetPosition().col == targetStart)
			{
				foundStart = true;
			}

			if (currentPiece && currentPiece->GetPosition().col == targetEnd)
			{
				foundEnd = true;
			}
		}

		for (const auto& neighbor : currentPiece->GetNeighbors())
		{
			if (!visited.contains(neighbor->GetPosition()))
			{
				stack.push(neighbor->GetPosition());
			}
		}
	}

	return foundStart && foundEnd;
}

std::vector<Position> Board::GetChain(const Position& start) const
{
	//make sure the position is valid
	if (!IsPositionValid(start))
	{
		throw GameException("Invalid position");
	}
	//make sure the position is occupied
	if (m_board[start.row][start.col] == nullptr)
	{
		throw GameException("Position is not occupied");
	}
	//do a dfs and return all the positions that are connected to the start position
	std::vector<Position> result;
	std::stack<Position> stack;
	std::set<Position> visited;

	stack.push(start);

	while (!stack.empty())
	{
		Position currentPos = stack.top();
		stack.pop();

		if (!IsPositionValid(currentPos) || visited.contains(currentPos) == true)
		{
			continue;
		}

		visited.insert(currentPos);

		const IPiecePtr currentPiece = At(currentPos);

		result.push_back(currentPos);

		for (const auto& neighbor : currentPiece->GetNeighbors())
		{
			if (!visited.contains(neighbor->GetPosition()))
			{
				stack.push(neighbor->GetPosition());
			}
		}
	}

	return result;
}

std::set<std::vector<Position>> Board::GetChains(EColor playerColor) const
{
	//return all the chains on the board
	std::set<std::vector<Position>> result;
	std::set<Position> visited;
	for (int i = 0; i < m_size; i++)
	{
		for (int j = 0; j < m_size; j++)
		{
			const Position pos(i, j);
			if (m_board[i][j] != nullptr && visited.contains(pos) == 0 && m_board[i][j]->GetColor() == playerColor)
			{
				std::vector<Position> chain = GetChain(pos);
				result.insert(chain);
				for (const auto& pos : chain)
				{
					visited.insert(pos);
				}
			}
		}
	}
	return result;
}

ILinkPtr& Board::GetLinkBetween(Position pos1, Position pos2)
{
	for (auto& link : m_links)
	{
		if ((link->GetPiece1()->GetPosition() == pos1 && link->GetPiece2()->GetPosition() == pos2) ||
			(link->GetPiece1()->GetPosition() == pos2 && link->GetPiece2()->GetPosition() == pos1))
		{
			return link;
		}
	}
	throw GameException("No link between the two pieces");
}

void Board::AddLink(const ILinkPtr& link)
{
	m_links.push_back(link);
}

void Board::RemoveLink(const ILinkPtr& link)
{
	for (auto it = m_links.begin(); it != m_links.end(); ++it)
	{
		if (*it == link)
		{
			m_links.erase(it);
			return;
		}
	}
	throw GameException("Link not found");
}

std::vector<ILinkPtr> Board::GetLinks() const
{
	return m_links;
}

std::vector<IPiecePtr> Board::GetPieces() const
{
	std::vector<IPiecePtr> result;
	for (const auto& row : m_board)
	{
		for (const auto& piece : row)
		{
			if (piece != nullptr)
			{
				result.push_back(piece);
			}
		}
	}
	return result;
}

bool Board::CheckIfWinningPlacement(const ILinkPtr& link) const
{
	const IPiecePtr piece = link->GetPiece1();
	const EColor color = piece->GetColor();

	/*
	if (color == EColor::Red)
	{
		return CheckPathToRows(piece->GetPosition(), 0, m_size - 1);
	}

	if (color == EColor::Black) 
	{
		return CheckPathToCols(piece->GetPosition(), 0, m_size - 1);
	}*/

	return CheckPath(piece->GetPosition(), 0, m_size - 1, color);
}
