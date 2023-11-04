#pragma once

#include <stdexcept>;

// GameException -> logic_error
// OutOfBoundsException -> GameException
// InvalidMoveException -> GameException

export class GameException : public std::logic_error
{
public:
	GameException(const std::string& message) : std::logic_error(message) {}
};