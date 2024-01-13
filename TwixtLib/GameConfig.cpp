#include "GameConfig.h"

#include <fstream>

GameConfig::GameConfig(const std::string& fileName)
{
	size_t currentLine = 0;
	std::ifstream file(fileName);

	if(!file.is_open())
	{
		throw std::runtime_error("Could not open file");
	}

	std::string line;
	while (std::getline(file, line))
	{
		switch  (currentLine)
		{
			case 0:
				m_boardSize = std::stoi(line);
				break;
			case 1:
				m_boardString = line;
				m_boardString += "\n";
				break;
			case 2:
				m_playerOneLinks = line;
				m_playerOneLinks += "\n";
				break;
			case 3:
				m_playerTwoLinks = line;
				m_playerTwoLinks += "\n";
				break;
			case 4:
				m_turn = line;
				m_turn += "\n";
				break;
			case 5:
				m_state = line;
				m_state += "\n";
				break;
			case 6:
				m_playerPegLimit = std::stoi(line);
				break;
			case 7:
				m_playerLinkLimit = std::stoi(line);
				break;
			default:
				break;
		}
		currentLine++;
	}
}

GameConfig::GameConfig(int boardSize, std::string boardString, std::string playerOneLinks, std::string playerTwoLinks, std::string turn, std::string state, int playerPegLimit, int playerLinkLimit)
	: m_boardSize(boardSize)
	, m_boardString(std::move(boardString))
	, m_playerOneLinks(std::move(playerOneLinks))
	, m_playerTwoLinks(std::move(playerTwoLinks))
	, m_turn(std::move(turn))
	, m_state(std::move(state))
	, m_playerPegLimit(playerPegLimit)
	, m_playerLinkLimit(playerLinkLimit)
{}

int GameConfig::GetBoardSize() const
{
	return m_boardSize;
}

std::string GameConfig::GetBoardString() const
{
	return m_boardString;
}

std::string GameConfig::GetPlayerOneLinks() const
{
	return m_playerOneLinks;
}

std::string GameConfig::GetPlayerTwoLinks() const
{
	return m_playerTwoLinks;
}

std::string GameConfig::GetTurn() const
{
	return m_turn;
}

std::string GameConfig::GetState() const
{
	return m_state;
}

int GameConfig::GetPlayerPegLimit() const
{
	return m_playerPegLimit;
}

int GameConfig::GetPlayerLinkLimit() const
{
	return m_playerLinkLimit;
}
