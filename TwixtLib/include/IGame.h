#pragma once

#include "EColor.h"
#include "IPiece.h"
#include "IGameListener.h"
#include "IPlayer.h"

#include <memory>
#include <string>


using IGamePtr = std::shared_ptr<class IGame>;

class IGame
{
public:
	static IGamePtr CreateGame();
	virtual ~IGame() = default;
	virtual void AddListener(IGameListenerPtr listener) = 0;
	virtual void RemoveListener(IGameListener* listener) = 0;
	virtual void Reset() = 0;
	virtual void PlacePiece(const Position& pos) = 0;
	virtual void CreateLink(const Position& pos1, const Position& pos2) = 0;
	virtual void RemoveLink(const Position& pos1, const Position& pos2) = 0;
	virtual void Recommend() = 0;
	virtual void SwitchTurn()=0;
	[[nodiscard]] virtual EColor GetCurrentPlayerColor() const = 0;
	[[nodiscard]] virtual IPiecePtr GetPiecePtr(const Position& pos) const = 0;
	[[nodiscard]] virtual int GetPegsLimitNumber(EColor playerColor) = 0;
	[[nodiscard]] virtual int GetLinksLimitNumber(EColor playerColor) = 0;
	[[nodiscard]] virtual int GetAvailablePegsNumber(EColor playerColor) const = 0;
	[[nodiscard]] virtual int GetAvailableLinksNumber(EColor playerColor) const = 0;
	[[nodiscard]] virtual int GetBoardSize() const = 0;
	[[nodiscard]] virtual bool IsGameOver() const = 0;
	[[nodiscard]] virtual bool IsDraw() const = 0;
	[[nodiscard]] virtual bool IsWon() const = 0;
	virtual void LoadFromFile(const std::string& fileName) = 0;
	virtual void SaveToFile(const std::string& fileName) const = 0;
	virtual void EndInDraw() const = 0;
	virtual void ReconfigureGame(int boardSize, int maxPegs, int maxLinks) = 0;
};
