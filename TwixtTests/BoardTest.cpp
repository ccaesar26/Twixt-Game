#include "gtest/gtest.h"

#include "Board.h"
#include "GameException.h"

class BoardTest : public testing::Test {
protected:
    void SetUp() override
	{
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

TEST_F(BoardTest, TestAtOutOfBounds)
{
	EXPECT_EQ(b1.At(Position(0, 0)), nullptr);
	EXPECT_EQ(b1.At(Position(11, 11)), nullptr);
    EXPECT_THROW(b1.At(Position(-1, -1)), GameException);
    EXPECT_THROW(b1.At(Position(11,30)), GameException);
}

TEST_F(BoardTest, TestPlacePieceOutOfBounds)
{
    EXPECT_THROW(b1.PlacePiece(Position(1, 1), EColor::Red), GameException);
    EXPECT_THROW(b1.PlacePiece(Position(1,1), EColor::Black), GameException);
	EXPECT_THROW(b1.PlacePiece(Position(-1, -1), EColor::Black), GameException);
	EXPECT_THROW(b1.PlacePiece(Position(11, 30), EColor::Red), GameException);
}

TEST_F(BoardTest, TestSize)
{
    EXPECT_EQ(b1.GetSize(), 10);
}

TEST_F(BoardTest, TestIsLinked)
{
	b1.LinkPieces(Position(1, 1), Position(2, 3));
	Link link = b1.GetLinkBetween(Position(1, 1), Position(2, 3));
    EXPECT_EQ(link.GetPiece1()->GetPosition(), Position(1, 1));
    EXPECT_EQ(link.GetPiece2()->GetPosition(), Position(2, 3));
}

TEST_F(BoardTest, TestIsLinkedOutOfBounds)
{
    EXPECT_THROW(b1.LinkPieces(Position(1, 1), Position(11, 11)), GameException);
    EXPECT_THROW(b1.LinkPieces(Position(1, 1), Position(-1, -1)), GameException);
    EXPECT_THROW(b1.LinkPieces(Position(1, 1), Position(11, 30)), GameException);
    EXPECT_THROW(b1.LinkPieces(Position(10, 10), Position(11, 30)), GameException);
}

TEST_F(BoardTest, TestIsLinkedNotAdjacent)
{
    b1.PlacePiece(Position(1, 2), EColor::Black);
    b1.PlacePiece(Position(4, 5), EColor::Red);
	EXPECT_THROW(b1.LinkPieces(Position(1, 1), Position(1, 2)), GameException);
	EXPECT_THROW(b1.LinkPieces(Position(1, 2), Position(1, 1)), GameException);
	EXPECT_THROW(b1.LinkPieces(Position(4, 4), Position(4, 5)), GameException);
	EXPECT_THROW(b1.LinkPieces(Position(4, 5), Position(4, 4)), GameException);
}

TEST_F(BoardTest, TestIsLinkedSameColor)
{
    b1.PlacePiece(Position(5, 6), EColor::Black);
    b1.PlacePiece(Position(3, 5), EColor::Red);
    EXPECT_THROW(b1.LinkPieces(Position(2, 3), Position(3, 5)), GameException);
    EXPECT_THROW(b1.LinkPieces(Position(3, 5), Position(2, 3)), GameException);
    EXPECT_THROW(b1.LinkPieces(Position(5, 6), Position(4, 4)), GameException);
    EXPECT_THROW(b1.LinkPieces(Position(4, 4), Position(5, 6)), GameException);
}
