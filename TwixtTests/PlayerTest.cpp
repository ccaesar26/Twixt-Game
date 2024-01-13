#include "gtest/gtest.h"
#include "Link.h"
#include "Player.h"
#include "GameException.h"
#include "Board.h"
#include "Peg.h"

TEST(PlayerTests, CreatePlayer)
{

	IBoardWeakPtr b;
	IBoardPtr b1 = IBoard::CreateBoard();
	b = b1;
	if (b.lock())
	{
		std::string name = "Vlad";
		Player player(EColor::Black, name, b,50,50);
		EXPECT_EQ(player.GetName(), name);
	}
}

TEST(PlayerTests, CreatePlayer2)
{
	IBoardWeakPtr b;
	IBoardPtr b1 = IBoard::CreateBoard();
	b = b1;
	if (b.lock())
	{
		std::string name = "Vlad";
		IPlayerPtr player = Player::CreatePlayer(EColor::Black, name, b, 50, 50);
		EXPECT_EQ(player->GetColor(), EColor::Black);
	}
}

TEST(PlayerTests, AddPeg)
{
	IBoardWeakPtr b;
	IBoardPtr b1 = IBoard::CreateBoard();
	b = b1;
	if (b.lock())
	{
		std::string name = "Vlad";
		Player player(EColor::Black, name, b, 50, 50);
		IPiecePtr peg1 = IPiece::Produce(EColor::Black, Position(1, 1));
		IPiecePtr peg2 = IPiece::Produce(EColor::Black, Position(2, 1));
		player.AddPeg(peg1);
		player.AddPeg(peg2);
		EXPECT_EQ(player.GetPegs().size(), 2);
	}
}

TEST(PlayerTests, AddLink)
{
	IBoardWeakPtr b;
	IBoardPtr b1 = IBoard::CreateBoard();
	b = b1;
		if (b.lock())
		{
		std::string name = "Vlad";
		Player player(EColor::Black, name, b, 50, 50);
		IPiecePtr peg1 = IPiece::Produce(EColor::Black, Position(1, 1));
		IPiecePtr peg2 = IPiece::Produce(EColor::Black, Position(2, 3));
		ILinkPtr link = ILink::Produce(peg1, peg2, EColor::Black);
		player.AddLink(link);
		EXPECT_EQ(player.GetLinks().size(), 1);
	    }
}

TEST(PlayerTests, RemoveLink)
{
	IBoardWeakPtr b;
	IBoardPtr b1 = IBoard::CreateBoard();
	b = b1;
	if (b.lock())
	{
		std::string name = "Vlad";
		Player player(EColor::Black, name, b, 50, 50);
		IPiecePtr peg1 = IPiece::Produce(EColor::Black, Position(1, 1));
		IPiecePtr peg2 = IPiece::Produce(EColor::Black, Position(2, 3));
		ILinkPtr link = ILink::Produce(peg1, peg2, EColor::Black);
		player.AddLink(link);
		player.RemoveLink(link);
		EXPECT_EQ(player.GetLinks().size(), 0);
	}
}