#pragma once

#include "EColor.h"

using IPiecePtr = std::shared_ptr<class IPiece>;

class IPiece
{
public:
	static IPiecePtr Produce(EColor color);
	virtual ~IPiece() = default;
	virtual EColor GetColor() const = 0;
	virtual bool isNorthLinked() const = 0;
	virtual bool isSouthLinked() const = 0;
};