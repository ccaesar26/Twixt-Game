export module gameException;

import <stdexcept>;

namespace twixt
{
	// GameException -> logic_error
	// OutOfBoundsException -> GameException
	// InvalidMoveException -> GameException

	export class GameException : public std::logic_error
	{

	};
}