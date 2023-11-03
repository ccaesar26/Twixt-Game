#include "Board.h"

IBoardPtr IBoard::CreateBoard()
{
	return std::make_shared<Board>();
}