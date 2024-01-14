#pragma once

#include "IBoard.h"
#include "EColor.h"
#include "IPiece.h"
#include "ILink.h"
#include "GameConfig.h"

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <array>

class Board final :
	public IBoard
{
public:
	explicit Board(int size = 24);

	Board(const std::string& boardString, const std::string& playerOneLinks, const std::string& playerTwoLinks, int size = 24, const std::
		function<void(Position pos1, Position pos2, EColor color)>& notificationCallback = [](const Position p1, const Position p2, const EColor color){});

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

	std::array<std::vector<Position>, 2> GetPotentialNeighbours(const Position& pos) override;

	void LinkPieces(Position pos1, Position pos2) override;

	void UnlinkPieces(Position pos1, Position pos2) override;

	[[nodiscard]] IPiecePtr At(const Position pos) const override;

	[[nodiscard]] bool IsPositionValid(const Position& pos) const;

	[[nodiscard]] bool CheckIfWinningPlacement(const ILinkPtr& link) const override;

	[[nodiscard]] int GetSize() const override;

	[[nodiscard]] std::string ToString() const override;

	[[nodiscard]] std::string LinksToString(EColor playerColor) const override;

	[[nodiscard]] bool CheckPath(const Position& pos, int targetStart, int targetEnd, EColor playerColor) const;

	[[nodiscard]] std::vector<Position> GetChain(const Position& start) const;

	[[nodiscard]] std::set<std::vector<Position>> GetChains(EColor playerColor) const override;

	[[nodiscard]] ILinkPtr& GetLinkBetween(Position pos1, Position pos2) override;

	void AddLink(const ILinkPtr& link);

	void RemoveLink(const ILinkPtr& link);

	[[nodiscard]] std::vector<ILinkPtr> GetLinks() const override;
	
	[[nodiscard]] std::vector<IPiecePtr> GetPieces() const override;
private:
	int m_size;
	std::vector<std::vector<IPiecePtr>> m_board;
	std::vector<ILinkPtr> m_links;
};
