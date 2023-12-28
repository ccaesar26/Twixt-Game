#pragma once
#include "IPiece.h"

class Link final
{
	private:
	IPiecePtr m_piece1;
	IPiecePtr m_piece2;
	EColor m_color;
	public:
	Link(IPiecePtr piece1, IPiecePtr piece2, EColor color);
	[[nodiscard]] IPiecePtr GetPiece1() const;
	[[nodiscard]] IPiecePtr GetPiece2() const;
	[[nodiscard]] EColor GetColor() const;
	[[nodiscard]] bool operator==(const Link& other) const;
	[[nodiscard]] bool operator!=(const Link& other) const;
};
