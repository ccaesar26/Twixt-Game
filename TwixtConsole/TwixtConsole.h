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
private:
	IGamePtr m_game;
	


};

