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

void Peg::AddNeighbor(const IPiecePtr piece)
{
	m_neighbors.push_back(piece);
}

void Peg::RemoveNeighbor(const IPiecePtr piece)
{
	for (int i = 0; i < m_neighbors.size(); i++)
	{
		if (m_neighbors[i] == piece)
		{
			m_neighbors.erase(m_neighbors.begin() + i);
			break;
		}
	}
}

Position Peg::GetPosition() const
{
	return m_position;
}
