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


