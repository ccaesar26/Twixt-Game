#include "gtest/gtest.h"
#include "Link.h"
#include "Player.h"
#include "GameException.h"
#include "Board.h"

TEST(PlayerTests, CreatePlayer)
{
	IBoardWeakPtr b;
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
		if (b.lock())
		{
		std::string name = "Vlad";
		Player player(EColor::Black, name, b);
		player.AddPeg(Position(1, 1));
		player.AddPeg(Position(2, 3));
		player.AddLink(Position(1, 1), Position(2, 3));
		EXPECT_EQ(player.GetLinks().size(), 1);
	}
}