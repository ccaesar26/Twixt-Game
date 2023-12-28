#include "Player.h"
#include <memory>

IPlayerPtr IPlayer::CreatePlayer(EColor color, std::string name, Board& board)
{
	return std::make_unique<Player>(color, name, board);
}

Player::Player(EColor color, std::string name, Board& board) :
	m_color{ color },
	m_name{ std::move(name) },
	m_board{ board }
{}

std::vector<IPiecePtr> Player::GetPegs() const
{
	return m_pegs;
}

std::vector<std::reference_wrapper<Link>> Player::GetLinks() const
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
	m_board.PlacePiece(pos, m_color);
	m_pegs.push_back(m_board.At(pos));
}

void Player::AddLink(Position pos1, Position pos2)
{
	m_board.LinkPieces(pos1, pos2);
	Link& link = m_board.GetLinkBetween(pos1, pos2);
	m_links.emplace_back(link);
}

void Player::RemoveLink(IPiecePtr peg1, IPiecePtr peg2)
{
	// TODO: Implement
}
