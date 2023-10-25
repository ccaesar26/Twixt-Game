#pragma once

class IGame
{
public:
	static IGamePtr CreateGame();
	~IGame() = default;
	virtual void AddListener(IGameListenerPtr listener) = 0;
	virtual void RemoveListener(IGameListenerPtr listener) = 0;
}