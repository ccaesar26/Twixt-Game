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

private:
	IGamePtr m_game;
	


};

