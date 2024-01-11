#include "gtest/gtest.h"
#include "Game.h"
#include "GameException.h"
#include "Link.h"

TEST(GameTest, GameCreation)
{
	std::shared_ptr game = IGame::CreateGame();

	EXPECT_EQ(game==NULL,false );
}

TEST(GameTest, GameFirstTurn)
{
	std::shared_ptr game = IGame::CreateGame();
	EXPECT_EQ(game->GetCurrentPlayerColor(), EColor::Red);
	
}
	
TEST(GameTest, GameSecondTurn)
{
	std::shared_ptr game = IGame::CreateGame();
	game->PlacePiece(Position(1, 1));
	game->SwitchTurn();
	EXPECT_EQ(game->GetCurrentPlayerColor(), EColor::Black);

}

TEST(GameTest, GamePiecePlace)
{
		std::shared_ptr game = IGame::CreateGame();
	game->PlacePiece(Position(1, 1));
	EXPECT_EQ(game->GetPiecePtr(Position(1, 1))->GetColor(), EColor::Red);
}
	
TEST(GameTest, GameReset)
{
    std::shared_ptr game = IGame::CreateGame();
    game->SwitchTurn();
    std::shared_ptr game1 = IGame::CreateGame();
    game->Reset();
    EXPECT_EQ(game->GetCurrentPlayerColor(), game1->GetCurrentPlayerColor());

}