#pragma once

#include "collectible.h"

class guava : public collectible
{
public:
	guava(Map* map, float x, float y) : collectible("sprites/guava.png", map, x, y)
	{
		this->groupId = 6;
	}
};