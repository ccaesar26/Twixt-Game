#pragma once

#include "EColor.h"
#include "IPiece.h"

class ILink
{
public:
	virtual ~ILink() = default;

	[[nodiscard]] virtual IPiecePtr GetPiece1() const = 0;
	[[nodiscard]] virtual IPiecePtr GetPiece2() const = 0;
	[[nodiscard]] virtual EColor GetColor() const = 0;
};


