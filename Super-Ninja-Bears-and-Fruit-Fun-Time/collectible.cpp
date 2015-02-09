#include "main_game.h"
#include "collectible.h"

collectible::collectible(std::string gfx, Map* map, float x, float y)
{
	this->position = sf::Vector2f(x, y);
	this->Load(gfx);
	this->setPosition(x, y);
	this->map = map;
	this->groupId = 6;
}

void collectible::offset(float x, float y)
{
	this->move(x, y);
	this->position.x += x;
	this->position.y += y;
}

bool collectible::Update(sf::RenderWindow* window)
{
	this->setPosition(this->position.x - Entity::scroll.x, this->position.y - Entity::scroll.y);
	Entity::Update(window);
	return true;
}
