#include "Board.h"
#include "GameException.h"
#include <stack>
#include "Peg.h"

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
	return false;
}

bool Board::CheckPathToCols(const Position pos, int targetLeftCol, int targetRightCol) const
{
	return false;
}

bool Board::CheckIfWinningPlacement(Position pos, EColor currentPlayer) const
{
	return false;
}
