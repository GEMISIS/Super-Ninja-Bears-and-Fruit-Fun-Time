#include "main_game.h"
#include "base_npc.h"

base_npc::base_npc(Speech* speech, std::string gfx, Map* map, std::string text, float x, float y)
{
	this->position = sf::Vector2f(x, y);
	this->Load(gfx);
	this->setPosition(x, y);
	this->map = map;
	this->groupId = 3;
	this->speech = speech;
	this->text = text;
}

void base_npc::offset(float x, float y)
{
	this->move(x, y);
	this->position.x += x;
	this->position.y += y;
}

bool base_npc::Update(sf::RenderWindow* window)
{
	this->setPosition(this->position.x - Entity::scroll.x, this->position.y - Entity::scroll.y);
	Entity::Update(window);
	return true;
}

void base_npc::Collision(Entity* entity)
{
	switch (entity->GroupID())
	{
	case 2:
		speech->SetText(this->text);
		entity->Destroy();
		break;
	}
}

