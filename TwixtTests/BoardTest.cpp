#include "gtest/gtest.h"
#include "Link.h"
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
    EXPECT_EQ(b1.GetSize(), 24);
}

TEST_F(BoardTest, TestIsLinked)
{
	b1.LinkPieces(Position(1, 1), Position(2, 3));
    ILinkPtr link = b1.GetLinkBetween(Position(1, 1), Position(2, 3));
	
    EXPECT_EQ(link->GetPiece1()->GetPosition(), Position(1, 1));
    EXPECT_EQ(link->GetPiece2()->GetPosition(), Position(2, 3));
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

TEST_F(BoardTest, TestRemoveLink)
{
    b1.LinkPieces(Position(1, 1), Position(2, 3));
    b1.RemoveLink(b1.GetLinkBetween(Position(1, 1), Position(2, 3)));
    EXPECT_THROW(b1.GetLinkBetween(Position(1, 1), Position(2, 3)), GameException);
}

TEST_F(BoardTest, TestRemoveLinkOutOfBounds)
{
	EXPECT_THROW(b1.RemoveLink(b1.GetLinkBetween(Position(1, 1), Position(11, 11))), GameException);
	EXPECT_THROW(b1.RemoveLink(b1.GetLinkBetween(Position(1, 1), Position(-1, -1))), GameException);
	EXPECT_THROW(b1.RemoveLink(b1.GetLinkBetween(Position(1, 1), Position(11, 30))), GameException);
	EXPECT_THROW(b1.RemoveLink(b1.GetLinkBetween(Position(10, 10), Position(11, 30))), GameException);
}

TEST_F(BoardTest, TestLinkAfterRemove)
{
    b1.LinkPieces(Position(1, 1), Position(2, 3));
    b1.RemoveLink(b1.GetLinkBetween(Position(1, 1), Position(2, 3)));
    b1.LinkPieces(Position(1, 1), Position(2, 3));
    ILinkPtr link = b1.GetLinkBetween(Position(1, 1), Position(2, 3));
    EXPECT_EQ(link->GetPiece1()->GetPosition(), Position(1, 1));
    EXPECT_EQ(link->GetPiece2()->GetPosition(), Position(2, 3));
}

TEST_F(BoardTest, TestFunctionLink)
{
  
    Link link (b1.At(Position(1, 1)), b1.At(Position(2, 3)), EColor::Black);
    b1.LinkPieces(Position(1, 1), Position(2, 3));
    EXPECT_EQ(link.GetPiece1()->GetPosition(), Position(1, 1));
    EXPECT_EQ(link.GetPiece2()->GetPosition(), Position(2, 3));
}


TEST_F(BoardTest, TestFunctionCheckPathToRows)
{
    b1.PlacePiece(Position(5, 6), EColor::Red);
    b1.PlacePiece(Position(6, 8), EColor::Red);
    b1.PlacePiece(Position(7, 7), EColor::Red);
    b1.PlacePiece(Position(4, 8), EColor::Red);
    b1.PlacePiece(Position(8, 9), EColor::Red);
    b1.LinkPieces(Position(5, 6), Position(6, 8));
    b1.LinkPieces(Position(8, 9), Position(6, 8));
    b1.LinkPieces(Position(5, 6), Position(7, 7));
    b1.LinkPieces(Position(4, 4), Position(5, 6));
    b1.LinkPieces(Position(5, 6), Position(4, 8));
    EXPECT_EQ(b1.CheckPath(Position(5, 6), 4, 8, EColor::Red), true);
    EXPECT_EQ(b1.CheckPath(Position(4, 4), 4, 9, EColor::Red), false);
    EXPECT_EQ(b1.CheckPath(Position(4, 4), 3, 8, EColor::Red), false);

}

TEST_F(BoardTest, TestFunctionCheckPathToCols)
{
    b1.PlacePiece(Position(3, 5), EColor::Black);
    b1.LinkPieces(Position(1, 1), Position(2, 3));
    b1.LinkPieces(Position(2, 3), Position(3, 5));

    EXPECT_EQ(b1.CheckPath(Position(1, 1), 1, 5, EColor::Black), true);
    EXPECT_EQ(b1.CheckPath(Position(1, 1), 0, 5, EColor::Black), false);
    EXPECT_EQ(b1.CheckPath(Position(1, 1), 1, 6, EColor::Black), false);
}

TEST_F(BoardTest, TestFunctionGetLinks)
{
    b1.PlacePiece(Position(4, 7), EColor::Black);
    b1.PlacePiece(Position(3, 5), EColor::Black);
    b1.LinkPieces(Position(1, 1), Position(2, 3));
    b1.LinkPieces(Position(2, 3), Position(3, 5));
    b1.LinkPieces(Position(3, 5), Position(4, 7));
    ILinkPtr link1 = b1.GetLinkBetween(Position(1, 1), Position(2, 3));
    ILinkPtr link2 = b1.GetLinkBetween(Position(2, 3), Position(3, 5));
    ILinkPtr link3 = b1.GetLinkBetween(Position(3, 5), Position(4, 7));

    EXPECT_EQ(link1->GetPiece1(), b1.GetLinkBetween(Position(1, 1), Position(2, 3))->GetPiece1());
    EXPECT_EQ(link2->GetPiece1(), b1.GetLinkBetween(Position(2, 3), Position(3, 5))->GetPiece1());
    EXPECT_EQ(link3->GetPiece1(), b1.GetLinkBetween(Position(3, 5), Position(4, 7))->GetPiece1());
}

TEST_F(BoardTest, TestFunctionGetLinkBetween)
{
    b1.PlacePiece(Position(3, 5), EColor::Black);
    b1.LinkPieces(Position(1, 1), Position(2, 3));
    b1.LinkPieces(Position(2, 3), Position(3, 5));

    EXPECT_EQ(b1.GetLinkBetween(Position(1, 1), Position(2, 3))->GetPiece1()->GetPosition(), Position(1, 1));
    EXPECT_EQ(b1.GetLinkBetween(Position(1, 1), Position(2, 3))->GetPiece2()->GetPosition(), Position(2, 3));

}

TEST_F(BoardTest, TestFunctionGetLinkBetween2DifferentColors)
{
    b1.PlacePiece(Position(3, 5), EColor::Red);
    b1.LinkPieces(Position(1, 1), Position(2, 3));

    EXPECT_THROW(b1.GetLinkBetween(Position(2, 3), Position(3, 5))->GetPiece2()->GetPosition(), GameException);
    EXPECT_THROW(b1.GetLinkBetween(Position(2, 3), Position(5, 5))->GetPiece1()->GetPosition(), GameException);
}

TEST_F(BoardTest, TestUnlinkPieces)
{
    b1.LinkPieces(Position(1, 1), Position(2, 3));
    b1.UnlinkPieces(Position(1, 1), Position(2, 3));
    EXPECT_THROW(b1.GetLinkBetween(Position(1,1),Position(2,3)), GameException);
}

TEST_F(BoardTest, TestFunctionCheckIfWinningPlacement)
{
    b1.LinkPieces(Position(1, 1), Position(2, 3));
    ILinkPtr link = b1.GetLinkBetween(Position(1, 1), Position(2, 3));
    EXPECT_EQ(b1.CheckIfWinningPlacement(link), false);
}

TEST_F(BoardTest, TestLinkIntersection)
{
    b1.LinkPieces(Position(1, 1), Position(2, 3));
    b1.PlacePiece(Position(2, 2),EColor::Black);
    b1.PlacePiece(Position(1, 4),EColor::Black);
    EXPECT_THROW(b1.LinkPieces(Position(2, 2), Position(1, 4)), GameException);
    EXPECT_THROW(b1.LinkPieces(Position(1, 4), Position(2, 2)), GameException);
}


TEST_F(BoardTest, TestFunctionCheckIfWinningPlacement2)
{
    b1.PlacePiece(Position(0, 1), EColor::Red);
    b1.PlacePiece(Position(2, 2), EColor::Red);
    b1.PlacePiece(Position(4, 1), EColor::Red);
    b1.PlacePiece(Position(6, 2), EColor::Red);
    b1.PlacePiece(Position(8, 1), EColor::Red);
    b1.PlacePiece(Position(10, 2), EColor::Red);
    b1.PlacePiece(Position(12, 1), EColor::Red);
    b1.PlacePiece(Position(14, 2), EColor::Red);
    b1.PlacePiece(Position(16, 1), EColor::Red);
    b1.PlacePiece(Position(18, 2), EColor::Red);
    b1.PlacePiece(Position(20, 1), EColor::Red);
    b1.PlacePiece(Position(22, 2), EColor::Red);
    b1.PlacePiece(Position(23, 4), EColor::Red);
    b1.LinkPieces(Position(0, 1), Position(2, 2));
    b1.LinkPieces(Position(2, 2), Position(4, 1));
    b1.LinkPieces(Position(4, 1), Position(6, 2));
    b1.LinkPieces(Position(6, 2), Position(8, 1));
    b1.LinkPieces(Position(8, 1), Position(10, 2));
    b1.LinkPieces(Position(10, 2), Position(12, 1));
    b1.LinkPieces(Position(12, 1), Position(14, 2));
    b1.LinkPieces(Position(14, 2), Position(16, 1));
    b1.LinkPieces(Position(16, 1), Position(18, 2));
    b1.LinkPieces(Position(18, 2), Position(20, 1));
    b1.LinkPieces(Position(20, 1), Position(22, 2));
    b1.LinkPieces(Position(22, 2), Position(23, 4));
	ILinkPtr link = b1.GetLinkBetween(Position(0, 1), Position(2, 2));
	EXPECT_EQ(b1.CheckIfWinningPlacement(link), true);
}

TEST_F(BoardTest, TestFunctionCheckIfWinningPlacement3)
{
   
    b1.PlacePiece(Position(1, 0), EColor::Black);
    b1.PlacePiece(Position(2, 2), EColor::Black);
    b1.PlacePiece(Position(1, 4), EColor::Black);
    b1.PlacePiece(Position(2, 6), EColor::Black);
    b1.PlacePiece(Position(1, 8), EColor::Black);
    b1.PlacePiece(Position(2, 10), EColor::Black);
    b1.PlacePiece(Position(1, 12), EColor::Black);
    b1.PlacePiece(Position(2, 14), EColor::Black);
    b1.PlacePiece(Position(1, 16), EColor::Black);
    b1.PlacePiece(Position(2, 18), EColor::Black);
    b1.PlacePiece(Position(1, 20), EColor::Black);
    b1.PlacePiece(Position(2, 22), EColor::Black);
    b1.PlacePiece(Position(4, 23), EColor::Black);
    b1.LinkPieces(Position(1, 0), Position(2, 2));
    b1.LinkPieces(Position(2, 2), Position(1, 4));
    b1.LinkPieces(Position(1, 4), Position(2, 6));
    b1.LinkPieces(Position(2, 6), Position(1, 8));
    b1.LinkPieces(Position(1, 8), Position(2, 10));
    b1.LinkPieces(Position(2, 10), Position(1, 12));
    b1.LinkPieces(Position(1, 12), Position(2, 14));
    b1.LinkPieces(Position(2, 14), Position(1, 16));
    b1.LinkPieces(Position(1, 16), Position(2, 18));
    b1.LinkPieces(Position(2, 18), Position(1, 20));
    b1.LinkPieces(Position(1, 20), Position(2, 22));
    b1.LinkPieces(Position(2, 22), Position(4, 23));
    ILinkPtr link = b1.GetLinkBetween(Position(2, 22), Position(4, 23));
    EXPECT_EQ(b1.CheckIfWinningPlacement(link), true);

}

TEST_F(BoardTest, TestCheckIfPegPlacedOnWrongSides)
{
    EXPECT_THROW(b1.PlacePiece(Position(0, 1), EColor::Black), GameException);
    EXPECT_THROW(b1.PlacePiece(Position(23, 1), EColor::Black), GameException);
    EXPECT_THROW(b1.PlacePiece(Position(1, 0), EColor::Red), GameException);
    EXPECT_THROW(b1.PlacePiece(Position(1, 23), EColor::Red), GameException);
}

TEST_F(BoardTest, TestPegPlacedOnCorner)
{
    EXPECT_THROW(b1.PlacePiece(Position(0, 0), EColor::Black), GameException);
    EXPECT_THROW(b1.PlacePiece(Position(23, 23), EColor::Black), GameException);
    EXPECT_THROW(b1.PlacePiece(Position(0, 23), EColor::Red), GameException);
    EXPECT_THROW(b1.PlacePiece(Position(23, 0), EColor::Red), GameException);
    
}
TEST_F(BoardTest, TestLinkOperators)
{
	Link link1 = Link(b1.At(Position(1, 1)), b1.At(Position(2, 3)), EColor::Black);
    Link link2 = Link(b1.At(Position(2, 3)), b1.At(Position(3, 5)), EColor::Black);
    Link link3 = link2;
	EXPECT_EQ(link2==link3, true);
    EXPECT_EQ(link1!=link2, true);
    EXPECT_EQ(link1.GetColor(), EColor::Black);
	
}

TEST_F(BoardTest, TestOperator)
{
   
    Board b2 = b1;
    EXPECT_EQ(b2.At(Position(1, 1))->GetColor(), EColor::Black);
}

TEST_F(BoardTest, TestCopyConstructor)
{
	Board b2(b1);
	EXPECT_EQ(b2.At(Position(1, 1))->GetColor(), EColor::Black);
}

TEST_F(BoardTest, TestMoveConstructor)
{
	Board b2(std::move(b1));
	EXPECT_EQ(b2.At(Position(1, 1))->GetColor(), EColor::Black);
}

TEST_F(BoardTest, TestMoveOperator)
{
	Board b2;
	b2 = std::move(b1);
	EXPECT_EQ(b2.At(Position(1, 1))->GetColor(), EColor::Black);
}