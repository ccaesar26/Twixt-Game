#pragma once
#include "IPlayer.h"

class Player final :
	public IPlayer
{
private:
	std::vector<IPiecePtr> m_pegs;
	std::vector<Links> m_links;
	EColor m_color;
	std::string m_name;
public:
	explicit Player(EColor color, std::string name);
	[[nodiscard]] std::vector<IPiecePtr> GetPegs() const override;
	[[nodiscard]] std::vector<Links> GetLinks() const override;
	[[nodiscard]] EColor GetColor() const override;
	[[nodiscard]] std::string GetName() const override;
	void AddPeg(IPiecePtr peg);
	void AddLink(IPiecePtr peg1, IPiecePtr peg2);
	void RemoveLink(IPiecePtr peg1, IPiecePtr peg2);
};