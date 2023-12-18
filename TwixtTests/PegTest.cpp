#include "gtest/gtest.h"

#include "Peg.h"

TEST(PegCreation, ColorTest)
{
	Peg p(EColor::Black,Position (1,1));
	EXPECT_EQ(p.GetColor(), EColor::Black);
}

TEST(PegCreation, PositionTest)
{
	Peg p(EColor::Black, Position(1, 1));
	Position pos(1, 1);
	EXPECT_EQ(p.GetPosition(), pos);
}