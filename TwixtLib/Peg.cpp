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

bool Peg::isNorthLinked() const
{
	return m_isNorthLinked;
}

bool Peg::isSouthLinked() const
{
	return m_isSouthLinked;
}

EColor Peg::GetColor() const
{
	return m_color;
}
