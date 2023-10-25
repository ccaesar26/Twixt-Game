module;

#include "IPiece.h"
#include "Position.h"

export module iBoard;

import std;

namespace twixt
{
	export using IBoardPtr = std::shared_ptr<class IBoard>;

	export class IBoard
	{
	public:
		static IBoardPtr CreateBoard();

		virtual void PlacePiece(Position pos, EColor color) = 0;

		virtual IPiecePtr at(Position pos) const = 0;

		virtual bool CheckIfWinningPlacement(Position pos, EColor currentPlayer) const = 0;
	};
}