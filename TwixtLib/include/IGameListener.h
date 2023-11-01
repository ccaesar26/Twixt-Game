#pragma once

class IGameListener {
public:
	virtual void OnPiecePlaced(const Position& pos) = 0;
	virtual void OnGameOver(const EGameResult& result) = 0;
	virtual void OnGameRestarted() = 0;
};