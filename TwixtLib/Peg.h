#pragma once

#include <vector>
#include <memory>

#include "IPiece.h"
#include <Position.h>

class Peg final :
	public IPiece
{
private:
	EColor m_color;
	Position m_position;
	bool m_isNorthLinked;
	bool m_isSouthLinked;
	std::vector<IPiecePtr> m_neighbors;

public:
	explicit Peg(EColor color, Position pos);
	[[nodiscard]] bool IsNorthLinked() const override;
	[[nodiscard]] bool IsSouthLinked() const override;
	[[nodiscard]] EColor GetColor() const override;
	std::vector<IPiecePtr> GetNeighbors() const override;
	void AddNeighbor(IPiecePtr piece) override;
	void RemoveNeighbor(IPiecePtr piece) override;
	Position GetPosition() const override;
};
