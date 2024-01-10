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
	static IPlayerPtr CreatePlayer(EColor color, std::string name, IBoardWeakPtr board);
	[[nodiscard]] virtual std::vector<IPiecePtr> GetPegs() const = 0;
	[[nodiscard]] virtual std::vector<ILinkWeakPtr> GetLinks() const = 0;
	[[nodiscard]] virtual EColor GetColor() const = 0;
	[[nodiscard]] virtual std::string GetName() const = 0;
};
