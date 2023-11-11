#include "Peg.h"

IPiecePtr IPiece::Produce(EColor color)
{
	return std::make_shared<Peg>(color);
}

Peg::Peg(EColor color)
{
	m_color = color;
	m_isNorthLinked = false;
	m_isSouthLinked = false;
}

