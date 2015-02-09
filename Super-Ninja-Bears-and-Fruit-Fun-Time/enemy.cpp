#include "main_game.h"
#include "enemy.h"

enemy::enemy(Map* map, std::string gfx, float x, float y, sf::IntRect rect)
{
	this->position = sf::Vector2f(x, y);
	this->Load(gfx, rect);
	this->setPosition(x, y);
	this->map = map;
	this->groupId = 5;
	this->health = 1;
	this->velocity = sf::Vector2f(0, 0);
}

void enemy::offset(float x, float y)
{
	this->position.x += x;
	this->position.y += y;
}

bool enemy::Update(sf::RenderWindow* window)
{
	this->setPosition(this->position.x - Entity::scroll.x, this->position.y - Entity::scroll.y);
	if (this->health <= 0)
	{
		this->Destroy();
	}
	this->offset(this->velocity.x, this->velocity.y);
	return true;
}

void enemy::Collision(Entity* entity)
{
	switch (entity->GroupID())
	{
	case 4:
		entity->Destroy();
		this->health -= 1;
		break;
	}
}

