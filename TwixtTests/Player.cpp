#include "gtest/gtest.h"
#include "Link.h"
#include "Player.h"
#include "GameException.h"
#include "Board.h"

TEST(PlayerTests, CreatePlayer)
{
	IBoardWeakPtr b;
	std::string name = "Vlad";
	Player player(EColor::Black,name,b);
	EXPECT_EQ(player.GetName(), name);
}

TEST(PlayerTests, CreatePlayer2)
{
	IBoardWeakPtr b;
	std::string name = "Vlad";
	IPlayerPtr player= Player::CreatePlayer(EColor::Black, name, b);
	EXPECT_EQ(player->GetName(), name);
}