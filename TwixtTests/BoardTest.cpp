#include "gtest/gtest.h"

#include "Board.h"

class BoardTest : public testing::Test {
protected:
    void SetUp() override {
        b1.PlacePiece(Position(1, 1), EColor::Black);
        b1.PlacePiece(Position(2, 3), EColor::Black);
        b1.PlacePiece(Position(4, 4), EColor::Red);
        b1.PlacePiece(Position(10, 10), EColor::Red);
        
    }
    // void TearDown() override {}


    Board  b1;
};

TEST_F(BoardTest,PlacePiece)
{
    EXPECT_EQ(b1.At(Position(1, 1))->GetColor(), EColor::Black);
    EXPECT_EQ(b1.At(Position(2, 3))->GetColor(), EColor::Black);
    EXPECT_EQ(b1.At(Position(4, 4))->GetColor(), EColor::Red);
    EXPECT_EQ(b1.At(Position(10, 10))->GetColor(), EColor::Red);

}