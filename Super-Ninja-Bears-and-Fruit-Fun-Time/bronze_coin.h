#pragma once

#include <SFML/Audio.hpp>

#include "collectible.h"

class bronze_coin : public collectible
{
public:
	bronze_coin(Map* map, float x, float y) : collectible("sprites/coinBronze.png", map, x, y)
	{
		this->groupId = 7;
	}

	void Destory()
	{
		Entity::Destroy();
	}

	~bronze_coin()
	{
	}
private:
};