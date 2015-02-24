#pragma once

#include "entity.h"
#include "map.h"
#include "SaveSystem.h"

extern SaveSystem saveSystem;;

class enemy : public Entity
{
public:
	enemy(Map* map, std::string gfx, float x, float y, int health, int damage, sf::IntRect rect = sf::IntRect());
	void offset(float x, float y);
	virtual bool Update(sf::RenderWindow* window);
	void Collision(Entity* entity);

	inline int Health()
	{
		return this->health;
	}

	inline int Damage()
	{
		return this->damage;
	}

protected:
	Map* map;
	int health, damage;
	sf::Vector2f position;
};

