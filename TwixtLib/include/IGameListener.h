#pragma once

#include "Position.h"
#include "EGameResult.h"
#include "EColor.h"

#include <memory>

using IGameListenerPtr = std::shared_ptr<class IGameListener>;
using IGameListenerWeakPtr = std::weak_ptr<class IGameListener>;

class IGameListener
{
public:
	virtual ~IGameListener() = default;
	virtual void OnBoardChanged(int newSize, int newMaxPegs, int newMaxLinks) = 0;
	virtual void OnPiecePlaced(const Position& pos) = 0;
	virtual void OnLinkPlaced(const Position& pos1, const Position& pos2) = 0;
	virtual void OnLinkRemoved(const Position& pos1, const Position& pos2) = 0;
	virtual void OnHintRecommended(std::pair<Position, Position> link) = 0;
	virtual void OnGameOver(const EGameResult& result) = 0;
	virtual void OnGameRestarted() = 0;
	virtual void OnGameLoaded() = 0;
	virtual void OnDrawRequested(EColor current_player) = 0;
};
