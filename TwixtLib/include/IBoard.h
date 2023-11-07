#pragma once

#include "IPiece.h"
#include "Position.h"

#include <memory>

using IBoardPtr = std::shared_ptr<class IBoard>;

class IBoard
{
public:
	static IBoardPtr CreateBoard();

	virtual void PlacePiece(Position pos, EColor color) = 0;

	virtual IPiecePtr At(Position pos) const = 0;

	virtual bool CheckIfWinningPlacement(Position pos, EColor currentPlayer) const = 0;
};