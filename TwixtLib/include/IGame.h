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
	virtual void Restore(const std::string& boardString, const std::string& turn, const std::string& state) = 0;
	virtual void PlacePiece(const Position& pos) = 0;
	virtual void CreateLink(const Position& pos1, const Position& pos2) = 0;
	virtual void RemoveLink(const Position& pos1, const Position& pos2) = 0;
	virtual void SwitchTurn()=0;
	[[nodiscard]] virtual EColor GetCurrentPlayerColor() const = 0;
	[[nodiscard]] virtual IPiecePtr GetPiecePtr(const Position& pos) const = 0;
	[[nodiscard]] virtual bool IsGameOver() const = 0;
	[[nodiscard]] virtual bool IsDraw() const = 0;
	[[nodiscard]] virtual bool IsWon() const = 0;
	virtual void LoadFromFile(const std::string& fileName) = 0;
	virtual void SaveToFile(const std::string& fileName) const = 0;
	virtual bool IsFileValid(const std::string& fileName) const = 0;
	virtual bool RegexValidate(const std::string& fileName) const = 0;
};
