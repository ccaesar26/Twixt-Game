#include <utility>
#include "Position.h"

Position::Position(int r, int c) : row(r), col(c){}

Position::Position(const std::pair<int, int>& p) : row(p.first), col(p.second) {}

Position& Position::operator=(const std::pair<int, int>& p)
{
	row = p.first;
	col = p.second;
	return *this;
}

Position::operator std::pair<int, int>() const
{
	return std::make_pair(row, col);
}

bool Position::operator==(const Position& other) const
{
	return row == other.row && col == other.col;
}

bool Position::operator!=(const Position& other) const
{
	return !(*this == other);
}

bool operator==(const std::pair<int, int>& p, const Position& pos)
{
	return p.first == pos.row && p.second == pos.col;
}

bool operator!=(const std::pair<int, int>& p, const Position& pos)
{
	return !(p == pos);
}
