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
	int m_limitPegs;
	int m_limitLinks;
public:
	explicit Player(EColor color, std::string name, IBoardWeakPtr board, int limitPegs, int limitLinks);
	[[nodiscard]] std::vector<IPiecePtr> GetPegs() const override;
	[[nodiscard]] std::vector<ILinkWeakPtr> GetLinks() const override;
	[[nodiscard]] EColor GetColor() const override;
	[[nodiscard]] std::string GetName() const override;
	void AddPeg(IPiecePtr peg) override;
	void AddLink(ILinkPtr link) override;
	void RemoveLink(ILinkPtr link) override;
	[[nodiscard]] int GetLimitPegs() const override;
	[[nodiscard]] int GetLimitLinks() const override;
};