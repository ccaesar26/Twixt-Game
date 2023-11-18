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

// TODO: 'inline' implementation of Position's methods
