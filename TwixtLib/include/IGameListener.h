#pragma once

#include "Position.h"
#include "EGameResult.h"

#include <memory>

using IGameListenerPtr = std::shared_ptr<class IGameListener>;

class IGameListener {
public:
	virtual void OnPiecePlaced(const Position& pos) = 0;
	virtual void OnGameOver(const EGameResult& result) = 0;
	virtual void OnGameRestarted() = 0;
};