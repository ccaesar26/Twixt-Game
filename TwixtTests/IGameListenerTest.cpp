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

