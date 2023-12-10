#pragma once

#include "IGame.h"
#include "IGameListener.h"
#include "EColor.h"
#include "EGameState.h"
#include "Board.h"
#include "Player.h"

#include <vector>
#include <string>

class Game final
	: public IGame
{
private:
	Board m_board;
	EColor m_turn;
	EGameState m_state;
	std::vector<IGameListenerWeakPtr> m_listeners;
	IPlayerPtr m_player1;
	IPlayerPtr m_player2;
	IPlayer* m_currentPlayer;
	IPlayer* m_nextPlayer;

	void InitializeGame();
	void InitializeGame(const std::string& config, EColor turn = EColor::Black);
	void SwitchTurn();

public:
	Game();
	void AddListener(IGameListenerPtr listener) override;
	void RemoveListener(IGameListener* listener) override;
	[[nodiscard]] IPlayer* GetPlayer1() const override;
	[[nodiscard]] IPlayer* GetPlayer2() const override;
	[[nodiscard]] EColor GetCurrentPlayerColor() const override;
	[[nodiscard]] IPlayer* GetCurrentPlayerPtr() const override;
	[[nodiscard]] IPlayer* GetNextPlayerPtr() const override;
	[[nodiscard]] IPiecePtr GetPiecePtr(const Position& pos) const override;
	[[nodiscard]] bool IsGameOver() const override;
	[[nodiscard]] bool IsDraw() const override;
	[[nodiscard]] bool IsWon() const override;
	void LoadFromFile(const std::string& fileName) override;
	void PlacePiece(const Position& pos) override;
	void Reset() override;
	void Restore(const std::string& config) const override;
	void SaveToFile(const std::string& fileName) const override;
	void NotifyPiecePlaced(const Position& pos) const override;
	void NotifyGameOver(EGameResult gameResult) const override;
	void NotifyGameRestarted() const override;
};