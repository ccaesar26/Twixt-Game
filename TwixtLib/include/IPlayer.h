#pragma once
#include "IPiece.h"
#include "ILink.h"
#include "IBoard.h"
#include <memory>
#include <vector>
#include <string>

using IPlayerPtr = std::unique_ptr<class IPlayer>;
using ILinkWeakPtr = std::weak_ptr<ILink>;
using IBoardWeakPtr = std::weak_ptr<IBoard>;

class IPlayer
{
public:
	virtual ~IPlayer() = default;
	static IPlayerPtr CreatePlayer(EColor color, const std::string& name, const IBoardWeakPtr& board, int pegs, int links);
	[[nodiscard]] virtual std::vector<IPiecePtr> GetPegs() const = 0;
	[[nodiscard]] virtual std::vector<ILinkWeakPtr> GetLinks() const = 0;
	[[nodiscard]] virtual EColor GetColor() const = 0;
	[[nodiscard]] virtual std::string GetName() const = 0;
	virtual void AddPeg(IPiecePtr peg) = 0;
	virtual void AddLink(ILinkPtr link) = 0;
	virtual void RemoveLink(ILinkPtr link) = 0;
	[[nodiscard]] virtual int GetLimitPegs() const = 0;
	[[nodiscard]] virtual int GetLimitLinks() const = 0;
	[[nodiscard]] virtual int GetAvailablePegs() const = 0;
	[[nodiscard]] virtual int GetAvailableLinks() const = 0;
};
