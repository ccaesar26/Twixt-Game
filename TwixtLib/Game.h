#pragma once

#include <vector>

#include "EColor.h"
#include "EGameState.h"

class Game
{
private:
	EColor m_turn;
	EGameState m_state;
	vector<IGameListenerWeakPtr> m_listeners;
	
	void InitializeGame();
	void InitializeGamr(const string& config, Ecolor turn = EColor::Black);
	
public:
	void AddListener(IGameListenerPtr listener) const override;
	EColor GetCurrentPlayer() const override;
	IPiecePtr GetPiecePtr(const Position& pos) const override;
	

};