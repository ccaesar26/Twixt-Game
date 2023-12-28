#pragma once
#include "IPiece.h"
#include "Board.h"
#include <memory>
#include <vector>
#include <string>

using IPlayerPtr = std::unique_ptr<class IPlayer>;

class IPlayer
{
public:
	virtual ~IPlayer() = default;
	static IPlayerPtr CreatePlayer(EColor color, std::string name, Board& board);
	[[nodiscard]] virtual std::vector<IPiecePtr> GetPegs() const = 0;
	[[nodiscard]] virtual std::vector<std::reference_wrapper<Link>> GetLinks() const = 0;
	[[nodiscard]] virtual EColor GetColor() const = 0;
	[[nodiscard]] virtual std::string GetName() const = 0;
};
