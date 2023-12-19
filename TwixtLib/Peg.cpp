#include "Peg.h"

IPiecePtr IPiece::Produce(EColor color, Position pos)
{
	return std::make_shared<Peg>(color, pos);
}

Peg::Peg(const EColor color, const Position pos)
{
	m_color = color;
	m_isNorthLinked = false;
	m_isSouthLinked = false;
	m_position = pos;
}

bool Peg::IsNorthLinked() const
{
	return m_isNorthLinked;
}

bool Peg::IsSouthLinked() const
{
	return m_isSouthLinked;
}

EColor Peg::GetColor() const
{
	return m_color;
}

std::vector<IPiecePtr> Peg::GetNeighbors() const
{
	return m_neighbors;
}

void Peg::AddNeighbor(IPiecePtr piece)
{
	m_neighbors.push_back(piece);
}

Position Peg::GetPosition() const
{
	return m_position;
}
