#include "gtest/gtest.h"
#include "Link.h"
#include "Player.h"
#include "GameException.h"
#include "Board.h"

TEST(PlayerTests, CreatePlayer)
{

	IBoardWeakPtr b;
	IBoardPtr b1 = IBoard::CreateBoard();
	b = b1;
	if (b.lock())
	{
		std::string name = "Vlad";
		Player player(EColor::Black, name, b);
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
		IPlayerPtr player = Player::CreatePlayer(EColor::Black, name, b);
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
		Player player(EColor::Black, name, b);
		player.AddPeg(Position(1, 1));
		player.AddPeg(Position(2, 1));
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
		Player player(EColor::Black, name, b);
		player.AddPeg(Position(1, 1));
		player.AddPeg(Position(2, 3));
		player.AddLink(Position(1, 1));
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
		Player player(EColor::Black, name, b);
		player.AddPeg(Position(1, 1));
		player.AddPeg(Position(2, 3));
		player.AddLink(Position(1, 1));
		player.RemoveLink(Position(1, 1));
		EXPECT_EQ(player.GetLinks().size(), 0);
	}
}