#pragma once

#include "IBoard.h"
#include "EColor.h"
#include "IPiece.h"
#include "ILink.h"

#include <vector>
#include <string>

class Board final :
	public IBoard
{
public:
	explicit Board(int size = 24);

	Board(const std::string& boardString, int size = 24);

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

	void LinkPieces(Position pos1, Position pos2) override;

	void UnlinkPieces(Position pos1, Position pos2) override;

	[[nodiscard]] IPiecePtr At(const Position pos) const override;

	[[nodiscard]] bool IsPositionValid(const Position& pos) const;

	[[nodiscard]] bool CheckIfWinningPlacement(const ILinkPtr& link) const override;

	[[nodiscard]] int GetSize() const;

	[[nodiscard]] std::string ToString() const override;

	[[nodiscard]] bool CheckPathToRows(const Position& pos, int targetUpperRow, int targetLowerRow) const;

	[[nodiscard]] bool CheckPathToCols(const Position& pos, int targetLeftCol, int targetRightCol) const;

	[[nodiscard]] ILinkPtr& GetLinkBetween(Position pos1, Position pos2) override;

	void AddLink(const ILinkPtr& link);

	void RemoveLink(const ILinkPtr& link);
private:
	int m_size;
	std::vector<std::vector<IPiecePtr>> m_board;
	std::vector<ILinkPtr> m_links;
};
