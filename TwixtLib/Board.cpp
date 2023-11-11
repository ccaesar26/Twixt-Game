#include "Board.h"

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