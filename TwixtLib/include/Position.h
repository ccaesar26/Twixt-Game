class Position {
public:
	int m_row;
	int m_col;

	Position(int r = 0, int c = 0);
	bool operator==(const Position& pos) const;
	bool operator!=(const Position& pos) const;
};