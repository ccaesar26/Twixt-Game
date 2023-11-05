#pragma once

#include "IGame.h"
#include "IGameListener.h"
#include "EColor.h"
#include "EGameState.h"
#include "GameException.h"
#include "Board.h"

#include <vector>
#include <string>

class Game
	: public IGame
{
private:
	Board m_board;
	EColor m_turn;
	EGameState m_state;
	std::vector<IGameListenerWeakPtr> m_listeners;
	
	void InitializeGame();
	void InitializeGamr(const std::string& config, EColor turn = EColor::Black);
	void SwitchTurn();
	
public:
	void AddListener(IGameListenerPtr listener) const override;
	EColor GetCurrentPlayer() const override;
	IPiecePtr GetPiecePtr(const Position& pos) const override;
	bool IsGameOver() const override;
	bool IsDraw() const override;
	bool IsWon() const override;
	void LoadFromFile(const std::string& fileName) const override;
	void PlacePiece(const Position& pos) override;
	void RemoveListener(IGameListenerPtr listener) const override;
	void Reset() override;
	void Restore(const std::string& config) const override;
	void SaveToFile(const std::string& fileName) const override;
	void NotifyPiecePlaced(Position& pos) const override;
	void NotifyGameOver(EGameResult gameResult) const override;
	void NotifyGameRestarted() const override;
};