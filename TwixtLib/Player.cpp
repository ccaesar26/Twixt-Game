#include "Player.h"
#include <memory>
#include <utility>

IPlayerPtr IPlayer::CreatePlayer(EColor color, const std::string& name, const IBoardWeakPtr& board, int pegs, int links)
{
	return std::make_unique<Player>(color, name, board, pegs, links);
}

Player::Player(EColor color, std::string name, IBoardWeakPtr board, int limitPegs, int limitLinks) :
	m_color{ color },
	m_name{ std::move(name) },
	m_board{std::move(board)},
	m_limitPegs{ limitPegs },
	m_limitLinks{ limitLinks },
	m_availablePegs{ limitPegs },
	m_availableLinks { limitLinks }
{}

std::vector<IPiecePtr> Player::GetPegs() const
{
	return m_pegs;
}

std::vector<ILinkWeakPtr> Player::GetLinks() const
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
	if(m_limitPegs>0)
	{
		m_pegs.push_back(std::move(peg));
		m_availablePegs--;
	}
}

void Player::AddLink(ILinkPtr link)
{
	if (m_limitLinks > 0)
	{
		ILinkWeakPtr weakLink = link;
		m_links.push_back(std::move(weakLink));
		m_availableLinks--;
	}
}

void Player::RemoveLink(ILinkPtr link)
{
	m_availableLinks++;
	std::erase_if(m_links, [link](const ILinkWeakPtr& linkWeak) {
		//increase the link limit
		return linkWeak.lock() == link;
	});
}

int Player::GetLimitPegs() const
{
	return m_limitPegs;
}

int Player::GetLimitLinks() const
{
	return m_limitLinks;
}

int Player::GetAvailablePegs() const
{
		return m_availablePegs;
}

int Player::GetAvailableLinks() const
{
		return m_availableLinks;
}
