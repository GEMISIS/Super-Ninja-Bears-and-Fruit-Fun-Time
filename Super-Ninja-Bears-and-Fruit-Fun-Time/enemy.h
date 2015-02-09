#pragma once

#include "entity.h"
#include "map.h"
#include "SaveSystem.h"

extern SaveSystem saveSystem;;

class enemy : public Entity
{
public:
	enemy(Map* map, std::string gfx, float x, float y, sf::IntRect rect = sf::IntRect());
	void offset(float x, float y);
	virtual bool Update(sf::RenderWindow* window);
	void Collision(Entity* entity);
protected:
	Map* map;
	int health;
	sf::Vector2f position;
};

