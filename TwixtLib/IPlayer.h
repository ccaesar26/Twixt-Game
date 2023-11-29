#pragma once

#include <memory>

using IPlayerPtr = std::shared_ptr<class IPlayer>;

class IPlayer
{
public:
	virtual ~IPlayer() = default;
	static IPlayerPtr CreatePlayer();
	
};
