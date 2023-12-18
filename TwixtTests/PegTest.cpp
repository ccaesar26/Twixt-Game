#include "gtest/gtest.h"

#include "Peg.h"

TEST(PegCreation, Color)
{
	Peg p(EColor::Black,Position (1,1));
	EXPECT_EQ(p.GetColor(), EColor::Black);
}