#pragma once
#include <string>

class GameConfig
{
private:
	int m_boardSize;
	std::string m_boardString;
	std::string m_playerOneLinks;
	std::string m_playerTwoLinks;
	std::string m_turn;
	std::string m_state;
	int m_playerPegLimit;
	int m_playerLinkLimit;
public:
	GameConfig() = default;

	GameConfig(const std::string& fileName);

	GameConfig(int boardSize, std::string boardString, std::string playerOneLinks, std::string playerTwoLinks, std::string turn, std::string state, int playerPegLimit, int playerLinkLimit);

	[[nodiscard]] int GetBoardSize() const;

	[[nodiscard]] std::string GetBoardString() const;

	[[nodiscard]] std::string GetPlayerOneLinks() const;

	[[nodiscard]] std::string GetPlayerTwoLinks() const;

	[[nodiscard]] std::string GetTurn() const;

	[[nodiscard]] std::string GetState() const;

	[[nodiscard]] int GetPlayerPegLimit() const;

	[[nodiscard]] int GetPlayerLinkLimit() const;
};

