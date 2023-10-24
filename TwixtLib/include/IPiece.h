#pragma once

class IPiece
{
	
	virtual ~IPiece() = default;
	EColor GetColor();
	bool isNorthLinked();
	bool isSouthLinked();

};