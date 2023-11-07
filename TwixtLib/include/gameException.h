#pragma once

#include <stdexcept>;

// GameException -> logic_error
// OutOfBoundsException -> GameException
// InvalidMoveException -> GameException

class GameException : public std::logic_error
{
public:
	GameException();
	GameException(const std::string& message);
};

inline GameException::GameException() : std::logic_error("Unknown error")
{
}

inline GameException::GameException(const std::string& message) : std::logic_error(message)
{
}
