#include "bullet.h"

Bullet::Bullet(Map* map, float x, float y, float direction, float distance)
{
	this->speed = 1;
	this->active = 1;
	this->groupId = 2;
	this->Load("sprites/shuriken.png");
	this->setColor(sf::Color::Transparent);
	this->velocity.x = cos(direction / 180.0f * 3.14f) * 3;
	this->velocity.y = sin(direction / 180.0f * 3.14f) * 3;
	position = sf::Vector2f(x + this->getGlobalBounds().width, y + this->getGlobalBounds().height);
	this->setPosition(this->position.x - Entity::scroll.x, this->position.y - Entity::scroll.y);
	this->setOrigin(this->getGlobalBounds().width / 2, this->getGlobalBounds().height / 2);
	this->setScale(0.5f, 0.5f);
	this->distance = distance;
	this->map = map;
}

bool Bullet::Update(sf::RenderWindow* window)
{
	this->position += this->velocity * this->speed;
	this->setPosition(this->position.x - Entity::scroll.x, this->position.y - Entity::scroll.y);
	this->rotate(this->velocity.x * this->speed);
	if (this->getPosition().y <= 0 || this->getPosition().y + this->getGlobalBounds().height >= window->getSize().y)
	{
		this->Destroy();
	}
	this->distance -= sqrt(this->velocity.x * this->velocity.x + this->velocity.y * this->velocity.y);
	if (this->distance <= 0)
	{
		this->Destroy();
	}
	if (this->map->CheckCollision(this, NONE).group != NO_GROUP && this->map->CheckCollision(this, NONE).group != WATER && this->map->CheckCollision(this, NONE).group != EXIT)
	{
		this->Destroy();
	}
	return true;
}

void Bullet::Collision(Entity* entity)
{
}
