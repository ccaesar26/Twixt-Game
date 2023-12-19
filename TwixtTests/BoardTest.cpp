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