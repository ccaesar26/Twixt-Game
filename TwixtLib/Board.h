#pragma once

#include "IBoard.h"
#include "EColor.h"
#include "IPiece.h"
#include "Peg.h"

#include <vector>
#include <string>

class Board final :
	public IBoard
{
public:
	explicit Board(int size = 24);

	Board(const std::string& boardString);

	// Rule of 5: Destructor
	~Board() override = default;

	// Rule of 5: Copy constructor
	Board(const Board& other);

	// Rule of 5: Copy assignment operator
	Board& operator=(const Board& other);

	// Rule of 5: Move constructor
	Board(Board&& other) noexcept;

	// Rule of 5: Move assignment operator
	Board& operator=(Board&& other) noexcept;

	void PlacePiece(Position pos, EColor color) override;

	[[nodiscard]] IPiecePtr At(Position pos) const override;

	[[nodiscard]] bool CheckIfWinningPlacement(Position pos, EColor currentPlayer) const override;

	[[nodiscard]] int GetSize() const;

	[[nodiscard]] std::string ToString() const;

private:
	int m_size;
	std::vector<std::vector<IPiecePtr>> m_board;
};
