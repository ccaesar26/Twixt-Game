#include "Board.h"
#include "GameException.h"
#include <stack>
#include "Peg.h"
#include <set>

IBoardPtr IBoard::CreateBoard()
{
	return std::make_shared<Board>();
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

Board::Board(const std::string& boardString, int size)
{
	size_t pos = 0;
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
		result += "\n";
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
	m_board[pos.row][pos.col] = IPiece::Produce(color, pos);
}

bool DoSegmentsIntersect(const Position& p1, const Position& p2, const Position& p3, const Position& p4)
{
	int d1 = (p3.col - p4.col) * (p1.row - p3.row) + (p3.row - p4.row) * (p3.col - p1.col);
	int d2 = (p3.col - p4.col) * (p2.row - p3.row) + (p3.row - p4.row) * (p3.col - p2.col);
	int d3 = (p1.col - p2.col) * (p3.row - p1.row) + (p1.row - p2.row) * (p1.col - p3.col);
	int d4 = (p1.col - p2.col) * (p4.row - p1.row) + (p1.row - p2.row) * (p1.col - p4.col);
	return d1 * d2 < 0 && d3 * d4 < 0;
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

	//check if the two positions are blocked by another link

	m_board[pos1.row][pos1.col]->AddNeighbor(m_board[pos2.row][pos2.col]);
	m_board[pos2.row][pos2.col]->AddNeighbor(m_board[pos1.row][pos1.col]);
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

bool Board::CheckPathToRows(const Position pos, int targetUpperRow, int targetLowerRow) const
{
	if (!IsPositionValid(pos)) {
		return false;
	}

	std::stack<Position> stack;

	std::set<Position> visited;

	stack.push(pos);

	bool foundUpperRow = false;
	bool foundLowerRow = false;

	while (!stack.empty() && !(foundUpperRow && foundLowerRow)) {
		Position currentPos = stack.top();
		stack.pop();

		if (!IsPositionValid(currentPos) || visited.contains(currentPos) > 0) {
			continue;
		}

		visited.insert(currentPos);

		const IPiecePtr currentPiece = At(currentPos);

		if (currentPiece && currentPiece->GetPosition().row == targetUpperRow) {
			foundUpperRow = true;
		}

		if (currentPiece && currentPiece->GetPosition().row == targetLowerRow) {
			foundLowerRow = true;
		}

		for (const auto& neighbor : currentPiece->GetNeighbors()) {
			if (!visited.contains(neighbor->GetPosition())) {
				stack.push(neighbor->GetPosition());
			}
		}
	}

	return foundUpperRow && foundLowerRow;
}

bool Board::CheckPathToCols(const Position pos, int targetLeftCol, int targetRightCol) const
{
	if (!IsPositionValid(pos)){
		return false;
	}

	std::stack<Position> stack;

	std::set<Position> visited;

	stack.push(pos);

	bool foundLeftCol = false;
	bool foundRightCol = false;

	while (!stack.empty() && !(foundLeftCol && foundRightCol)){
		Position currentPos = stack.top();
		stack.pop();

		if (!IsPositionValid(currentPos) || visited.contains(currentPos) > 0){
			continue;
		}

		visited.insert(currentPos);

		const IPiecePtr currentPiece = At(currentPos);

		if (currentPiece && currentPiece->GetPosition().col == targetLeftCol){
			foundLeftCol = true;
		}

		if (currentPiece && currentPiece->GetPosition().col == targetRightCol){
			foundRightCol = true;
		}

		for (const auto& neighbor : currentPiece->GetNeighbors()){
			if (!visited.contains(neighbor->GetPosition())){
				stack.push(neighbor->GetPosition());
			}
		}
	}

	return foundLeftCol && foundRightCol;
}

bool Board::CheckIfWinningPlacement(Position pos, EColor currentPlayer) const
{
	return false;
}
