#pragma once

#include <stdexcept>

class GameException : public std::logic_error
{
public:
	GameException();
	explicit GameException(const std::string& message);
};

inline GameException::GameException() : std::logic_error("Unknown error")
{
}

inline GameException::GameException(const std::string& message) : std::logic_error(message)
{
}

class OutOfBoundsException final : public GameException
{
public:
	OutOfBoundsException();
	explicit OutOfBoundsException(const std::string& message);
};

inline OutOfBoundsException::OutOfBoundsException() : GameException("Out of bounds")
{
}

inline OutOfBoundsException::OutOfBoundsException(const std::string& message) : GameException(message)
{
}

class InvalidMoveException final : public GameException
{
public:
	InvalidMoveException();
	explicit InvalidMoveException(const std::string& message);
};

inline InvalidMoveException::InvalidMoveException() : GameException("Invalid move")
{
}

inline InvalidMoveException::InvalidMoveException(const std::string& message) : GameException(message)
{
}

class InvalidStateException final : public GameException
{
public:
	InvalidStateException();
	explicit InvalidStateException(const std::string& message);
};

inline InvalidStateException::InvalidStateException() : GameException("Invalid state")
{
}

inline InvalidStateException::InvalidStateException(const std::string& message) : GameException(message)
{
}