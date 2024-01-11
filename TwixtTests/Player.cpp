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