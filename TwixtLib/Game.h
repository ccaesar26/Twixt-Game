#pragma once

#include "IGame.h"
#include "IGameListener.h"
#include "EColor.h"
#include "EGameState.h"
#include "Board.h"
#include "Player.h"
#include "GameConfig.h"

#include <vector>
#include <string>

class Game final
	: public IGame
{
private:
	IBoardPtr m_board;
	EColor m_turn;
	EGameState m_state;
	std::vector<IGameListenerWeakPtr> m_listeners;
	IPlayerPtr m_player1;
	IPlayerPtr m_player2;
	GameConfig m_gameConfig;

	void InitializeGame();
	void InitializeGame(const GameConfig& config);

public:
	void SwitchTurn() override;
	Game();
	void AddListener(IGameListenerPtr listener) override;
	void RemoveListener(IGameListener* listener) override;
	[[nodiscard]] EColor GetCurrentPlayerColor() const override;
	[[nodiscard]] IPiecePtr GetPiecePtr(const Position& pos) const override;
	[[nodiscard]] bool IsGameOver() const override;
	[[nodiscard]] bool IsDraw() const override;
	[[nodiscard]] bool IsWon() const override;
	void LoadFromFile(const std::string& fileName) override;
	void PlacePiece(const Position& pos) override;
	void CreateLink(const Position& pos1, const Position& pos2) override;
	void RemoveLink(const Position& pos1, const Position& pos2) override;
	std::pair<Position, std::vector<std::pair<Position, Position>>> Recommend() override;
	void Reset() override;
	void Restore(const GameConfig& config);
	void SaveToFile(const std::string& fileName) const override;
	void NotifyPiecePlaced(const Position& pos) const;
	void NotifyPiecesLinked(const Position& pos1, const Position& pos2) const;
	void NotifyLinkRemoved(const Position& pos1, const Position& pos2) const;
	void NotifyGameOver(EGameResult gameResult) const;
	void NotifyGameRestarted() const;
	bool IsFileValid(const std::string& fileName) const;
	bool RegexValidate(const std::string& fileName) const;
	void RequestDraw(EColor currentPlayer) const override;
	void NotifyDrawRequested(EColor m_turn) const;

};