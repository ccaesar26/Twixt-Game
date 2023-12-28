#pragma once

#include <memory>

#include "EColor.h"
#include <Position.h>
#include <vector>

using IPiecePtr = std::shared_ptr<class IPiece>;

class IPiece
{
public:
	static IPiecePtr Produce(EColor color, Position pos);
	virtual ~IPiece() = default;
	[[nodiscard]] virtual EColor GetColor() const = 0;
	[[nodiscard]] virtual bool IsNorthLinked() const = 0;
	[[nodiscard]] virtual bool IsSouthLinked() const = 0;
	[[nodiscard]] virtual std::vector<IPiecePtr> GetNeighbors() const = 0;
	[[nodiscard]] virtual void AddNeighbor(IPiecePtr piece) = 0;
	[[nodiscard]] virtual void RemoveNeighbor(IPiecePtr piece) = 0;
	[[nodiscard]] virtual Position GetPosition() const = 0;
};
