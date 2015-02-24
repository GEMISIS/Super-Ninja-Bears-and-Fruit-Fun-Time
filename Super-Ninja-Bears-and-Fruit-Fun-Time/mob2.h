#pragma once

#include "enemy.h"

class Mob2 : public enemy
{
public:
	Mob2(Map* map, float x, float y, int health, int damage) : enemy(map, "sprites/ninja b.png", x, y, health, damage)
	{
		this->health = 3;
	}
};