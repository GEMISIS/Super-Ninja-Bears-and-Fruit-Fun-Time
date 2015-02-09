#pragma once

#include <SFML/Audio.hpp>

#include "collectible.h"

class silver_coin : public collectible
{
public:
	silver_coin(Map* map, float x, float y) : collectible("sprites/coinSilver.png", map, x, y)
	{
		this->groupId = 8;
	}

	void Destory()
	{
		Entity::Destroy();
	}

	~silver_coin()
	{
	}
private:
};