#include "Link.h"

ILinkPtr ILink::Produce(const IPiecePtr& piece1, const IPiecePtr& piece2, EColor color)
{
	return std::make_shared<Link>(piece1, piece2, color);
}

Link::Link(IPiecePtr piece1, IPiecePtr piece2, EColor color) :
	m_piece1{std::move(piece1)},
	m_piece2{std::move(piece2)},
	m_color{ color }
{}

IPiecePtr Link::GetPiece1() const
{
	return m_piece1;
}

IPiecePtr Link::GetPiece2() const
{
	return m_piece2;
}

EColor Link::GetColor() const
{
	return m_color;
}

bool Link::operator==(const Link& other) const
{
	return m_piece1 == other.m_piece1 && m_piece2 == other.m_piece2 && m_color == other.m_color;
}

bool Link::operator!=(const Link& other) const
{
		return !(*this == other);
}
