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
	void PlacePiece(Position& pos);
	void PlaceBridge(Position& pos1, Position& pos2);
	void DeleteBridge(Position& pos1, Position& pos2);
	void EndTurn();
	void SaveToFile();
	void LoadFromFile();
	void SolicitDraw();

private:
	IGamePtr m_game;
	


};

