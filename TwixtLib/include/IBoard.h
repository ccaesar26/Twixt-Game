#pragma once

#include "IPiece.h"
#include "Position.h"

#include <memory>

using IBoardPtr = std::shared_ptr<class IBoard>;

class IBoard
{
public:
	virtual ~IBoard() = default;

	static IBoardPtr CreateBoard();

	virtual void PlacePiece(Position pos, EColor color) = 0;

	[[nodiscard]] virtual IPiecePtr At(Position pos) const = 0;

	[[nodiscard]] virtual bool CheckIfWinningPlacement(Position pos, EColor currentPlayer) const = 0;
};
