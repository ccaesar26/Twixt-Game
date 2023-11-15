#pragma once

#include "EColor.h"
#include "IPiece.h"
#include "IGameListener.h"

#include <memory>
#include <string>


using IGamePtr = std::shared_ptr<class IGame>;

class IGame
{
public:
	static IGamePtr CreateGame();
	virtual ~IGame() = default;
	virtual void AddListener(IGameListener* listener) = 0;
	virtual void RemoveListener(IGameListenerPtr listener) = 0;
	virtual void Reset() = 0;
	virtual void Restore(const std::string& config) const = 0;
	virtual void PlacePiece(const Position& pos) = 0;
	[[nodiscard]] virtual EColor GetCurrentPlayer() const = 0;
	[[nodiscard]] virtual IPiecePtr GetPiecePtr(const Position& pos) const = 0;
	[[nodiscard]] virtual bool IsGameOver() const = 0;
	[[nodiscard]] virtual bool IsDraw() const = 0;
	[[nodiscard]] virtual bool IsWon() const = 0;
	virtual void LoadFromFile(const std::string& fileName) const = 0;
	virtual void SaveToFile(const std::string& fileName) const = 0;
	virtual void NotifyPiecePlaced(const Position& pos) const = 0;
	virtual void NotifyGameOver(EGameResult gameResult) const = 0;
	virtual void NotifyGameRestarted() const = 0;
};
