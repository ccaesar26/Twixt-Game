#pragma once
#include "ILink.h"
#include "IPiece.h"

class Link final
	: public ILink 
{
public:
	Link(const IPiecePtr& piece1, const IPiecePtr& piece2, EColor color);
	Link(const Link& other) = default;
	[[nodiscard]] IPiecePtr GetPiece1() const override;
	[[nodiscard]] IPiecePtr GetPiece2() const override;
	[[nodiscard]] EColor GetColor() const override;
	[[nodiscard]] bool operator==(const Link& other) const;
	[[nodiscard]] bool operator!=(const Link& other) const;

private:
	IPiecePtr m_piece1;
	IPiecePtr m_piece2;
	EColor m_color;
};
