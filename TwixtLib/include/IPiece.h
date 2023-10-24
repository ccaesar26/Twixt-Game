#pragma once

class IPiece
{
public:
	virtual ~IPiece() = default;
	virtual EColor GetColor() = 0;
	virtual bool isNorthLinked() = 0;
	virtual bool isSouthLinked() = 0;

};