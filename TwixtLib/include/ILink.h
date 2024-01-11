#pragma once

#include "EColor.h"
#include "IPiece.h"

using ILinkPtr = std::shared_ptr<class ILink>;

class ILink
{
public:
	virtual ~ILink() = default;
	static ILinkPtr Produce(const IPiecePtr& piece1, const IPiecePtr& piece2, EColor color);
	[[nodiscard]] virtual IPiecePtr GetPiece1() const = 0;
	[[nodiscard]] virtual IPiecePtr GetPiece2() const = 0;
	[[nodiscard]] virtual EColor GetColor() const = 0;
};


