#pragma once

#include <IBoard.h>
#include "EColor.h"
#include "IPiece.h"
#include "Position.h"

#include <vector>

class Board :
	public IBoard
{
public:
	Board(int size = 24);

	virtual void PlacePiece(Position pos, EColor color) override;

	virtual IPiecePtr At(Position pos) const override;

	virtual bool CheckIfWinningPlacement(Position pos, EColor currentPlayer) const override;

private:
	int m_size;
	std::vector<std::vector<IPiecePtr>> m_board;
};
