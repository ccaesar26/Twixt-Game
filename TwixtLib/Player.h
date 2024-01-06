#pragma once
#include "Board.h"
#include "IPlayer.h"

class Player final :
	public IPlayer
{
private:
	std::vector<IPiecePtr> m_pegs;
	std::vector<std::reference_wrapper<ILink>> m_links;
	EColor m_color;
	std::string m_name;
	IBoard& m_board;
public:
	explicit Player(EColor color, std::string name, IBoard& board);
	[[nodiscard]] std::vector<IPiecePtr> GetPegs() const override;
	[[nodiscard]] std::vector<std::reference_wrapper<ILink>> GetLinks() const override;
	[[nodiscard]] EColor GetColor() const override;
	[[nodiscard]] std::string GetName() const override;
	void AddPeg(Position pos);
	void AddLink(Position pos1, Position pos2);
	void RemoveLink(Position peg1, Position peg2);
};