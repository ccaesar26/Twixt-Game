#include "gtest/gtest.h"
#include "Game.h"
#include "GameException.h"
#include "Link.h"

TEST(GameTest1, GameCreation)
{
	std::shared_ptr game = IGame::CreateGame();

	EXPECT_EQ(game==NULL,false );
}

TEST(GameTest1, GameFirstTurn)
{
	std::shared_ptr game = IGame::CreateGame();
	EXPECT_EQ(game->GetCurrentPlayerColor(), EColor::Red);
	
}
	
TEST(GameTest1, GameSecondTurn)
{
	std::shared_ptr game = IGame::CreateGame();
	game->PlacePiece(Position(1, 1));
	game->SwitchTurn();
	EXPECT_EQ(game->GetCurrentPlayerColor(), EColor::Black);

}

TEST(GameTest1, GamePiecePlace)
{
		std::shared_ptr game = IGame::CreateGame();
	game->PlacePiece(Position(1, 1));
	EXPECT_EQ(game->GetPiecePtr(Position(1, 1))->GetColor(), EColor::Red);
}
	
TEST(GameTest1, GameReset)
{
    std::shared_ptr game = IGame::CreateGame();
    game->SwitchTurn();
    std::shared_ptr game1 = IGame::CreateGame();
    game->Reset();
    EXPECT_EQ(game->GetCurrentPlayerColor(), game1->GetCurrentPlayerColor());

}

TEST(GameTest1, GameSave)
{
		std::shared_ptr game = IGame::CreateGame();
	game->PlacePiece(Position(1, 1));
	game->PlacePiece(Position(2, 3));
	game->CreateLink(Position(1, 1), Position(2, 3));
	std::string g ="TestInput.txt";
	game->SaveToFile(g);
	
}

TEST(GameTest1, GameLoad)
{
	std::shared_ptr game = IGame::CreateGame();
	game->PlacePiece(Position(1, 1));
	game->PlacePiece(Position(2, 3));
	game->CreateLink(Position(1, 1), Position(2, 3));
	std::string g = "TestInput.txt";
	game->SaveToFile(g);
	std::shared_ptr game1 = IGame::CreateGame();
	game1->LoadFromFile(g);
	EXPECT_EQ(game->GetCurrentPlayerColor(), game1->GetCurrentPlayerColor());
	EXPECT_EQ(game->GetPiecePtr(Position(1, 1))->GetColor(), game1->GetPiecePtr(Position(1, 1))->GetColor());
	EXPECT_EQ(game->GetPiecePtr(Position(2, 3))->GetColor(), game1->GetPiecePtr(Position(2, 3))->GetColor());
	
}