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
	IBoardPtr m_board;
	EColor m_turn;
	EGameState m_state;
	std::vector<IGameListenerWeakPtr> m_listeners;
	IPlayerPtr m_player1;
	IPlayerPtr m_player2;

	void InitializeGame();
	void InitializeGame(const std::string& boardString, const std::string& turn, const std::string& state);

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
	void Restore(const std::string& boardString, const std::string& turn, const std::string& state);
	void SaveToFile(const std::string& fileName) const override;
	void NotifyPiecePlaced(const Position& pos) const;
	void NotifyPiecesLinked(const Position& pos1, const Position& pos2) const;
	void NotifyLinkRemoved(const Position& pos1, const Position& pos2) const;
	void NotifyGameOver(EGameResult gameResult) const;
	void NotifyGameRestarted() const;
	bool IsFileValid(const std::string& fileName) const override;
	bool RegexValidate(const std::string& fileName) const;
	void RequestDraw() override;
	void NotifyDrawRequested(EColor m_turn) const;

};