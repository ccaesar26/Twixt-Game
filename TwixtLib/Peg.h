#pragma once

#include <vector>
#include <memory>

#include "IPiece.h"

class Peg :
    public IPiece
{
private:
    EColor m_color;
    bool m_isNorthLinked;
    bool m_isSouthLinked;
	std::vector<IPiecePtr> m_neighbours;

public:
	bool isNorthLinked() const override;
	bool isSouthLinked() const override;
	EColor GetColor() const override;
};

