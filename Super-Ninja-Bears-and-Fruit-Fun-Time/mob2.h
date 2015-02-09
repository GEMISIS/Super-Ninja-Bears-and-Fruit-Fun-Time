#pragma once

#include "enemy.h"

class Mob2 : public enemy
{
public:
	Mob2(Map* map, float x, float y) : enemy(map, "sprites/ninja b.png", x, y)
	{
		this->health = 3;
	}
};