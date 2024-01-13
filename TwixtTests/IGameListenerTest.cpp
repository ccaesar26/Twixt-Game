#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "IGameListener.h"
#include "Game.h"
#include "GameException.h"

using ::testing::InSequence;
using ::testing::_;


class MockListener : public IGameListener
{
public:
	MOCK_METHOD(void, OnPiecePlaced,(const Position& pos),(override));
	MOCK_METHOD(void, OnLinkPlaced, (const Position& pos1, const Position& pos2), (override));
	MOCK_METHOD(void, OnLinkRemoved, (const Position& pos1, const Position& pos2), (override));
	MOCK_METHOD(void, OnGameOver, (const EGameResult& result), (override));
	MOCK_METHOD(void, OnGameRestarted, (), (override));
	MOCK_METHOD(void, OnDrawRequested, (EColor current_player), (override));
};

TEST(OnPiecePlaced, LegalMove1)
{
	Game game;

	auto listener = std::make_shared<MockListener>();

	game.AddListener(listener);

	EXPECT_CALL(*listener, OnPiecePlaced(Position(1, 1)))
		.Times(1);

	game.PlacePiece(Position(1, 1));
}


TEST(OnPiecePlaced, IllegalMove1)
{
	Game game;

	auto listener = std::make_shared<MockListener>();

	game.AddListener(listener);

	EXPECT_CALL(*listener, OnPiecePlaced(Position(1, 1)))
		.Times(1);

	game.PlacePiece(Position(1, 1));
	try
	{
		EXPECT_THROW(game.PlacePiece(Position(1, 1)), GameException);
	}
	catch (const GameException& e)
	{
	}

}

TEST(OnLinkPlaced, LegalLinkPlace1)
{
	Game game;

	auto listener = std::make_shared<MockListener>();

	game.AddListener(listener);

	game.PlacePiece(Position(1, 1));
	game.PlacePiece(Position(2, 3));
	EXPECT_CALL(*listener, OnLinkPlaced(Position(1, 1), Position(2, 3)))
		.Times(1);
	game.CreateLink(Position(1, 1), Position(2, 3));
}

TEST(OnLinkPlaced, IllegalLinkPlace1)
{
	Game game;

	auto listener = std::make_shared<MockListener>();

	game.AddListener(listener);

	game.PlacePiece(Position(1, 1));
	game.PlacePiece(Position(2, 2));
	EXPECT_CALL(*listener, OnLinkPlaced(Position(1, 1), Position(2, 2)))
		.Times(0);
	try
	{
		EXPECT_THROW(game.CreateLink(Position(1, 1), Position(2, 2)), GameException);
	}
	catch (const GameException& e)
	{
	}
}

TEST(OnLinkPlaced, IllegalLinkPlace2)
{
	Game game;

	auto listener = std::make_shared<MockListener>();

	game.AddListener(listener);

	game.PlacePiece(Position(1, 1));
	game.SwitchTurn();
	game.PlacePiece(Position(2, 3));
	EXPECT_CALL(*listener, OnLinkPlaced(Position(1, 1), Position(2, 3)))
		.Times(0);
	try
	{
		EXPECT_THROW(game.CreateLink(Position(1, 1), Position(2, 3)), GameException);
	}
	catch (const GameException& e)
	{
	}
}

TEST(OnLinkRemoved, LegalLinkRemove1)
{
	Game game;

	auto listener = std::make_shared<MockListener>();

	game.AddListener(listener);

	game.PlacePiece(Position(1, 1));
	game.PlacePiece(Position(2, 3));
	game.CreateLink(Position(1, 1), Position(2, 3));
	EXPECT_CALL(*listener, OnLinkRemoved(Position(1, 1), Position(2, 3)))
		.Times(1);
	game.RemoveLink(Position(1, 1), Position(2, 3));
}

TEST(OnLinkRemoved, IllegalLinkRemove1)
{
	Game game;

	auto listener = std::make_shared<MockListener>();

	game.AddListener(listener);

	game.PlacePiece(Position(1, 1));
	game.SwitchTurn();
	game.PlacePiece(Position(2, 3));
	EXPECT_CALL(*listener, OnLinkRemoved(Position(1, 1), Position(2, 3)))
		.Times(0);
	try
	{
		EXPECT_THROW(game.RemoveLink(Position(1, 1), Position(2, 3)), GameException);
	}
	catch (const GameException& e)
	{
	}
}


TEST(OnGameOver, GameOver1)
{
	Game game;

	auto listener = std::make_shared<MockListener>();

	game.AddListener(listener);

	EXPECT_CALL(*listener, OnGameOver(EGameResult::RedWinner))
		.Times(1);
	game.PlacePiece(Position(0, 3));
	game.PlacePiece(Position(1, 1));
	game.PlacePiece(Position(2, 3));
	game.PlacePiece(Position(3, 1));
	game.PlacePiece(Position(4, 3));
	game.PlacePiece(Position(5, 1));
	game.PlacePiece(Position(6, 3));
	game.PlacePiece(Position(7, 1));
	game.PlacePiece(Position(8, 3));
	game.PlacePiece(Position(9, 1));
	game.PlacePiece(Position(10, 3));
	game.PlacePiece(Position(11, 1));
	game.PlacePiece(Position(12, 3));
	game.PlacePiece(Position(13, 1));
	game.PlacePiece(Position(14, 3));
	game.PlacePiece(Position(15, 1));
	game.PlacePiece(Position(16, 3));
	game.PlacePiece(Position(17, 1));
	game.PlacePiece(Position(18, 3));
	game.PlacePiece(Position(19, 1));
	game.PlacePiece(Position(20, 3));
	game.PlacePiece(Position(21, 1));
	game.PlacePiece(Position(22, 3));
	game.PlacePiece(Position(23, 1));
	game.CreateLink(Position(1, 1), Position(0, 3));
	game.CreateLink(Position(1, 1), Position(2, 3));
	game.CreateLink(Position(3, 1), Position(2, 3));
	game.CreateLink(Position(3, 1), Position(4, 3));
	game.CreateLink(Position(5, 1), Position(4, 3));
	game.CreateLink(Position(5, 1), Position(6, 3));
	game.CreateLink(Position(7, 1), Position(6, 3));
	game.CreateLink(Position(7, 1), Position(8, 3));
	game.CreateLink(Position(9, 1), Position(8, 3));
	game.CreateLink(Position(9, 1), Position(10, 3));
	game.CreateLink(Position(11, 1), Position(10, 3));
	game.CreateLink(Position(11, 1), Position(12, 3));
	game.CreateLink(Position(13, 1), Position(12, 3));
	game.CreateLink(Position(13, 1), Position(14, 3));
	game.CreateLink(Position(15, 1), Position(14, 3));
	game.CreateLink(Position(15, 1), Position(16, 3));
	game.CreateLink(Position(17, 1), Position(16, 3));
	game.CreateLink(Position(17, 1), Position(18, 3));
	game.CreateLink(Position(19, 1), Position(18, 3));
	game.CreateLink(Position(19, 1), Position(20, 3));
	game.CreateLink(Position(21, 1), Position(20, 3));
	game.CreateLink(Position(21, 1), Position(22, 3));
	game.CreateLink(Position(23, 1), Position(22, 3));
	
}

TEST(OnGameOver, GameOver2)
{
	Game game;

	auto listener = std::make_shared<MockListener>();

	game.AddListener(listener);
	game.SwitchTurn();
	EXPECT_CALL(*listener, OnGameOver(EGameResult::BlackWinner))
		.Times(1);
	game.PlacePiece(Position(1, 0));
	game.PlacePiece(Position(2, 2));
	game.PlacePiece(Position(1, 4));
	game.PlacePiece(Position(2, 6));
	game.PlacePiece(Position(1, 8));
	game.PlacePiece(Position(2, 10));
	game.PlacePiece(Position(1, 12));
	game.PlacePiece(Position(2, 14));
	game.PlacePiece(Position(1, 16));
	game.PlacePiece(Position(2, 18));
	game.PlacePiece(Position(1, 20));
	game.PlacePiece(Position(2, 22));
	game.PlacePiece(Position(4, 23));
	game.CreateLink(Position(1, 0), Position(2, 2));
	game.CreateLink(Position(2, 2), Position(1, 4));
	game.CreateLink(Position(1, 4), Position(2, 6));
	game.CreateLink(Position(2, 6), Position(1, 8));
	game.CreateLink(Position(1, 8), Position(2, 10));
	game.CreateLink(Position(2, 10), Position(1, 12));
	game.CreateLink(Position(1, 12), Position(2, 14));
	game.CreateLink(Position(2, 14), Position(1, 16));
	game.CreateLink(Position(1, 16), Position(2, 18));
	game.CreateLink(Position(2, 18), Position(1, 20));
	game.CreateLink(Position(1, 20), Position(2, 22));
	game.CreateLink(Position(2, 22), Position(4, 23));
}

TEST(OnGameOver, GameOver3)
{
	Game game;

	auto listener = std::make_shared<MockListener>();

	game.AddListener(listener);
	game.SwitchTurn();
	EXPECT_CALL(*listener, OnGameOver(EGameResult::Draw))
		.Times(1);
	game.RequestDraw(EColor::Black);
}	

TEST(OnGameRestarted, GameRestarted1)
{
	Game game;

	auto listener = std::make_shared<MockListener>();

	game.AddListener(listener);
	game.SwitchTurn();
	EXPECT_CALL(*listener, OnGameRestarted())
		.Times(1);
	game.Reset();
}