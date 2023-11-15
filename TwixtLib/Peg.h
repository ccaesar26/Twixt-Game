#pragma once

#include <vector>
#include <memory>

#include "IPiece.h"

class Peg final :
    public IPiece
{
private:
    EColor m_color;
    bool m_isNorthLinked;
    bool m_isSouthLinked;
	std::vector<IPiecePtr> m_neighbors;

public:
    explicit Peg(EColor color);
    [[nodiscard]] bool IsNorthLinked() const override;
    [[nodiscard]] bool IsSouthLinked() const override;
    [[nodiscard]] EColor GetColor() const override;
};