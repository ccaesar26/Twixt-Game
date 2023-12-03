#include "Board.h"
#include "GameException.h"

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

Board::Board(const std::string& boardString)
{
	size_t pos = 0;
	while (boardString[pos] != '\n')
	{
		if (boardString[pos] == ' ')
		{
			m_board[pos / m_size].push_back(nullptr);
		}
		else if (boardString[pos] == '0' || boardString[pos] == '1')
		{
			m_board[pos / m_size].push_back(std::make_shared<Peg>(static_cast<EColor>(boardString[pos] - '0')));
		}
		pos++;
	}
	m_size = static_cast<int>(m_board.size());
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
				m_board[i][j] = std::make_shared<Peg>(other.m_board[i][j]->GetColor());
			}
		}
	}
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
	m_board[pos.row][pos.col] = IPiece::Produce(color);
}

IPiecePtr Board::At(const Position pos) const
{
	if (pos.row < 0 || pos.row >= m_size || pos.col < 0 || pos.col >= m_size)
	{
		throw GameException("Invalid position");
	}
	return m_board[pos.row][pos.col];
}

bool Board::CheckIfWinningPlacement(Position pos, EColor currentPlayer) const
{
	return false;
}
