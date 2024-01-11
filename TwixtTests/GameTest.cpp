#include "gtest/gtest.h"
#include "Game.h"
#include "GameException.h"

TEST(GameTest, GameCreation)
{
	std::shared_ptr game = IGame::CreateGame();

	EXPECT_EQ(game==NULL,false );
}
	