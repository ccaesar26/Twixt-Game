#pragma once

#include <memory>

#include "EColor.h"

using IPiecePtr = std::shared_ptr<class IPiece>;

class IPiece
{
public:
	static IPiecePtr Produce(EColor color);
	virtual ~IPiece() = default;
	[[nodiscard]] virtual EColor GetColor() const = 0;
	[[nodiscard]] virtual bool IsNorthLinked() const = 0;
	[[nodiscard]] virtual bool IsSouthLinked() const = 0;
};