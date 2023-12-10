#pragma once
#include "IPiece.h"
#include <memory>
#include <vector>
#include <string>

using IPlayerPtr = std::unique_ptr<class IPlayer>;
using Links = std::pair<IPiecePtr, IPiecePtr>;

class IPlayer
{
public:
	virtual ~IPlayer() = default;
	static IPlayerPtr CreatePlayer(EColor color, std::string name);
	[[nodiscard]] virtual std::vector<IPiecePtr> GetPegs() const = 0;
	[[nodiscard]] virtual std::vector<Links> GetLinks() const = 0;
	[[nodiscard]] virtual EColor GetColor() const = 0;
	[[nodiscard]] virtual std::string GetName() const = 0;
};
