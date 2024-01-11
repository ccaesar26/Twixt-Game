#include "Player.h"
#include <memory>

IPlayerPtr IPlayer::CreatePlayer(EColor color, std::string name, IBoardWeakPtr board)
{
	return std::make_unique<Player>(color, name, board);
}

Player::Player(EColor color, std::string name, IBoardWeakPtr board) :
	m_color{ color },
	m_name{ std::move(name) },
	m_board{ board }
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

void Player::AddPeg(Position pos)
{
	if(const auto& board = m_board.lock())
	{
		board->PlacePiece(pos, m_color);
		m_pegs.push_back(board->At(pos));
	}
}

void Player::AddLink(Position pos1, Position pos2)
{
	if(const auto& board = m_board.lock())
	{
		board->LinkPieces(pos1, pos2);
		m_links.push_back(board->GetLinkBetween(pos1, pos2));
	}
}

void Player::RemoveLink(Position peg1, Position peg2)
{
	if(const auto& board = m_board.lock())
	{
		board->UnlinkPieces(peg1, peg2);
		erase_if(m_links, [&](const ILinkWeakPtr& link)
		{
			if(const auto& linkPtr = link.lock())
			{
				return linkPtr->GetPiece1()->GetPosition() == peg1 && linkPtr->GetPiece2()->GetPosition() == peg2;
			}
			return false;
		});
	}
}
