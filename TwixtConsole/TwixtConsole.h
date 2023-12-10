#pragma once
#include<iostream>
#include <utility>
#include "IGame.h"
#include "IGameListener.h"

class TwixtConsole : public IGameListener
{
public:
	TwixtConsole();
	void setGame(IGamePtr game);
	void printCurrentPlayer();
	void printHelperMenu();
	virtual void OnPiecePlaced(const Position& pos) override;
	virtual void OnGameOver(const EGameResult& result) override;
	virtual void OnGameRestarted() override;
	void printBoard();
	void PlacePiece(int x, int y);

private:
	IGamePtr m_game;
	


};

