#pragma once

#include "entity.h"
#include "map.h"

class base_npc : public Entity
{
public:
	base_npc(Speech* speech, std::string gfx, Map* map, std::string text, float x, float y);
	void offset(float x, float y);
	bool Update(sf::RenderWindow* window);
	void Collision(Entity* entity);
protected:
	std::string text;
	Map* map;
private:
	sf::Vector2f position;
	Speech* speech;
};

