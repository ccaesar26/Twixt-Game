#include "Board.h"
#include "GameException.h"

IBoardPtr IBoard::CreateBoard()
{
	return std::make_shared<Board>();
}

Board::Board(int size) : m_size(size) {
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

void Board::PlacePiece(Position pos, EColor color) {
	if (pos.m_row < 0 || pos.m_row >= m_size || pos.m_col < 0 || pos.m_col >= m_size) {
		throw GameException("Invalid position");
	}
	if (m_board[pos.m_row][pos.m_col] != nullptr) {
		throw GameException("Position is already occupied");
	}
	m_board[pos.m_row][pos.m_col] = IPiece::Produce(color);
}

IPiecePtr Board::At(Position pos) const {
	if (pos.m_row < 0 || pos.m_row >= m_size || pos.m_col < 0 || pos.m_col >= m_size) {
		throw GameException("Invalid position");
	}
	return m_board[pos.m_row][pos.m_col];
}