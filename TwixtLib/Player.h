#pragma once
#include "Board.h"
#include "IPlayer.h"

class Player final :
	public IPlayer
{
private:
	std::vector<IPiecePtr> m_pegs;
	std::vector<ILinkWeakPtr> m_links;
	EColor m_color;
	std::string m_name;
	IBoardWeakPtr m_board;
public:
	explicit Player(EColor color, std::string name, IBoardWeakPtr board);
	[[nodiscard]] std::vector<IPiecePtr> GetPegs() const override;
	[[nodiscard]] std::vector<ILinkWeakPtr> GetLinks() const override;
	[[nodiscard]] EColor GetColor() const override;
	[[nodiscard]] std::string GetName() const override;
	void AddPeg(Position pos);
	void AddLink(Position pos1, Position pos2);
	void RemoveLink(Position peg1, Position peg2);
};