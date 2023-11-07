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

class OutOfBoundsException : public GameException
{
public:
	OutOfBoundsException();
	OutOfBoundsException(const std::string& message);
};

inline OutOfBoundsException::OutOfBoundsException() : GameException("Out of bounds")
{
}

inline OutOfBoundsException::OutOfBoundsException(const std::string& message) : GameException(message)
{
}

class InvalidMoveException : public GameException
{
public:
	InvalidMoveException();
	InvalidMoveException(const std::string& message);
};

inline InvalidMoveException::InvalidMoveException() : GameException("Invalid move")
{
}

inline InvalidMoveException::InvalidMoveException(const std::string& message) : GameException(message)
{
}