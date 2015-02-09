#pragma once

#include "entity.h"
#include "map.h"

class collectible : public Entity
{
public:
	collectible(std::string gfx, Map* map, float x, float y);
	void offset(float x, float y);
	bool Update(sf::RenderWindow* window);
protected:
	Map* map;
private:
	sf::Vector2f position;
};

