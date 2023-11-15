#include "Board.h"
#include "GameException.h"

IBoardPtr IBoard::CreateBoard()
{
	return std::make_shared<Board>();
}

Board::Board(const int size) : m_size(size) {
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

void Board::PlacePiece(const Position pos, const EColor color) {
	if (pos.row < 0 || pos.row >= m_size || pos.col < 0 || pos.col >= m_size) {
		throw GameException("Invalid position");
	}
	if (m_board[pos.row][pos.col] != nullptr) {
		throw GameException("Position is already occupied");
	}
	m_board[pos.row][pos.col] = IPiece::Produce(color);
}

IPiecePtr Board::At(const Position pos) const {
	if (pos.row < 0 || pos.row >= m_size || pos.col < 0 || pos.col >= m_size) {
		throw GameException("Invalid position");
	}
	return m_board[pos.row][pos.col];
}