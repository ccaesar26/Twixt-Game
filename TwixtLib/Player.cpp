#include "Player.h"

Player::Player(EColor color, std::string name)
{
	m_color = color;
	m_name = std::move(name);
}

std::vector<IPiecePtr> Player::GetPegs() const
{
	return m_pegs;
}

std::vector<Links> Player::GetLinks() const
{
	return m_links;
}

EColor Player::GetColor() const
{
	return m_color;
}

std::string Player::GetName() const
{
	return m_name;
}

void Player::AddPeg(IPiecePtr peg)
{
	m_pegs.push_back(peg);
}

void Player::AddLink(IPiecePtr peg1, IPiecePtr peg2)
{
	m_links.emplace_back(peg1, peg2);
}

void Player::RemoveLink(IPiecePtr peg1, IPiecePtr peg2)
{
	auto it = std::find_if(m_links.begin(), m_links.end(), [peg1, peg2](const Links& link)
		{
			return (link.first == peg1 && link.second == peg2) || (link.first == peg2 && link.second == peg1);
		});

	if (it != m_links.end())
	{
		m_links.erase(it);
	}
}
