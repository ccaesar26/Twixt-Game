#pragma once

class Position {
public:
	int row;
	int col;

	explicit Position(int r = 0, int c = 0);
	bool operator==(const Position& pos) const;
	bool operator!=(const Position& pos) const;
};

// TODO: 'inline' implementation of Position's methods