#pragma once

#include <functional>

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
	static IBoardPtr CreateBoard(const std::string& config, const std::string& playerOneLinks, const std::string& playerTwoLinks, int size, const std::
	                             function<void(const Position pos1, const Position pos2, const EColor color)>&
	                             notificationCallback);

	virtual void PlacePiece(Position pos, EColor color) = 0;

	virtual void LinkPieces(const Position& pos1, const Position& pos2) = 0;

	virtual void UnlinkPieces(const Position& pos1, const Position& pos2) = 0;

	[[nodiscard]] virtual IPiecePtr At(const Position& pos) const = 0;

	[[nodiscard]] virtual bool CheckIfWinningPlacement(const ILinkPtr& link) const = 0;

	[[nodiscard]] virtual ILinkPtr& GetLinkBetween(const Position& pos1, const Position& pos2) = 0;

	[[nodiscard]] virtual std::string ToString() const = 0;

	[[nodiscard]] virtual std::string LinksToString(EColor playerColor) const = 0;

	[[nodiscard]] virtual std::set<std::vector<Position>> GetChains(EColor playerColor) const = 0;

	[[nodiscard]] virtual std::vector<ILinkPtr> GetLinks() const = 0;

	[[nodiscard]] virtual std::vector<IPiecePtr> GetPieces() const = 0;

	[[nodiscard]] virtual int GetSize() const = 0;

	[[nodiscard]] virtual std::array<std::vector<Position>, 2> GetPotentialNeighbours(
		const Position& pos) = 0;
};