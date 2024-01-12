#pragma once

#include "IPiece.h"
#include "ILink.h"
#include "Position.h"

#include <memory>
#include <string>
#include <set>

using IBoardPtr = std::shared_ptr<class IBoard>;

class IBoard
{
public:
	virtual ~IBoard() = default;

	static IBoardPtr CreateBoard();
	static IBoardPtr CreateBoard(int size);
	static IBoardPtr CreateBoard(const std::string& config, const std::string& playerOneLinks, const std::string& playerTwoLinks);

	virtual void PlacePiece(Position pos, EColor color) = 0;

	virtual void LinkPieces(Position pos1, Position pos2) = 0;

	virtual void UnlinkPieces(Position pos1, Position pos2) = 0;

	[[nodiscard]] virtual IPiecePtr At(const Position pos) const = 0;

	[[nodiscard]] virtual bool CheckIfWinningPlacement(const ILinkPtr& link) const = 0;

	[[nodiscard]] virtual ILinkPtr& GetLinkBetween(Position pos1, Position pos2) = 0;

	[[nodiscard]] virtual std::string ToString() const = 0;

	[[nodiscard]] virtual std::string LinksToString(EColor playerColor) const = 0;

	[[nodiscard]] virtual std::set<std::vector<Position>> GetChains(EColor playerColor) const = 0;

	[[nodiscard]] virtual std::vector<ILinkPtr> GetLinks() const = 0;

	[[nodiscard]] virtual std::vector<IPiecePtr> GetPieces() const = 0;
};