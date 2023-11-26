#pragma once

#include "IBoard.h"
#include "EColor.h"
#include "IPiece.h"

#include <vector>
#include <string>

class Board final :
	public IBoard
{
public:
	explicit Board(int size = 24);

	void PlacePiece(Position pos, EColor color) override;

	[[nodiscard]] IPiecePtr At(Position pos) const override;

	[[nodiscard]] bool CheckIfWinningPlacement(Position pos, EColor currentPlayer) const override;

	[[nodiscard]] int GetSize() const;

	[[nodiscard]] std::string ToString() const;

private:
	int m_size;
	std::vector<std::vector<IPiecePtr>> m_board;
};
