#pragma once
#include <utility>

class Position
{
public:
	int row;
	int col;

	//Default constructor
	explicit Position(int r = 0, int c = 0);
	
	//Conversion constructor from pair
	Position(const std::pair<int, int>& p);
	
	//Assignment operator from pair
	Position& operator=(const std::pair<int, int>& p);
	
	//Conversion operator to pair
	operator std::pair<int, int>() const;
	
	//Comparison operators
	bool operator==(const Position& other) const;
	bool operator!=(const Position& other) const;


};

//Comparison operators for comparing Position with std::pair<int, int>
bool operator==(const std::pair<int, int>& p, const Position& pos);
bool operator!=(const std::pair<int, int>& p, const Position& pos);


inline Position::Position(int r, int c) : row(r), col(c) {}
inline Position::Position(const std::pair<int, int>& p) : row(p.first), col(p.second) {}
inline Position& Position::operator=(const std::pair<int, int>& p)
{
	row = p.first;
	col = p.second;
	return *this;
}
inline Position::operator std::pair<int, int>() const
{
	return std::make_pair(row, col);
}
inline bool Position::operator==(const Position& other) const
{
	return row == other.row && col == other.col;
}
inline bool Position::operator!=(const Position& other) const
{
	return !(*this == other);
}
inline bool operator==(const std::pair<int, int>& p, const Position& pos)
{
	return p.first == pos.row && p.second == pos.col;
}
inline bool operator!=(const std::pair<int, int>& p, const Position& pos)
{
	return !(p == pos);
}
