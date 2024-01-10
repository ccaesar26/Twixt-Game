#pragma once

#include "IPiece.h"
#include "ILink.h"
#include "Position.h"

#include <memory>
#include <string>

using IBoardPtr = std::shared_ptr<class IBoard>;

class IBoard
{
public:
	virtual ~IBoard() = default;

	static IBoardPtr CreateBoard();
	static IBoardPtr CreateBoard(int size);
	static IBoardPtr CreateBoard(const std::string& config);

	virtual void PlacePiece(Position pos, EColor color) = 0;

	virtual void LinkPieces(Position pos1, Position pos2) = 0;

	virtual void UnlinkPieces(Position pos1, Position pos2) = 0;

	[[nodiscard]] virtual IPiecePtr At(const Position pos) const = 0;

	[[nodiscard]] virtual bool CheckIfWinningPlacement(Position pos, EColor currentPlayer) const = 0;

	[[nodiscard]] virtual ILinkPtr& GetLinkBetween(Position pos1, Position pos2) = 0;

	[[nodiscard]] virtual std::string ToString() const = 0;
};