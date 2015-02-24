#include "entity.h"

sf::Vector2f Entity::scroll(0, 0);

Entity::Entity()
{
	this->terminal_velocity = 10;
	this->texture = new sf::Texture();
	this->active = 1;
	this->groupId = 0;
	this->startFrame = 0;
	this->endFrame = 0;
	this->currentFrame = 0;
	this->fps = 16;
	this->xOffset = 0;
	this->yOffset = 0;
	this->Flip(false, false);
}

void Entity::Load(std::string filename, const sf::IntRect& area)
{
	this->texture->loadFromFile("Graphics/" + filename);
	this->setTexture(*this->texture);
	this->frameSize = area;
	this->timer.restart();

	if (this->frameSize.width <= 0 || this->frameSize.height <= 0)
	{
		this->frameSize = sf::IntRect(0, 0, this->getGlobalBounds().width, this->getGlobalBounds().height);
	}
	else
	{
		int left = (this->flippedH) ? (this->frameSize.left + this->xOffset) + this->frameSize.width : (this->frameSize.left + this->xOffset);
		int top = (this->flippedV) ? (this->frameSize.top + this->yOffset) + this->frameSize.height : (this->frameSize.top + this->yOffset);
		int width = (this->flippedH) ? -this->frameSize.width : this->frameSize.width;
		int height = (this->flippedV) ? -this->frameSize.height : this->frameSize.height;
		this->setTextureRect(sf::IntRect(left, top, width, height));
	}
	this->startFrame = 0;
	this->endFrame = 0;
	this->currentFrame = 0;
}

void Entity::SetFrameSize(sf::IntRect size)
{
	this->frameSize = size;
}

bool Entity::Update(sf::RenderWindow* window)
{
	this->move(this->velocity);
	return true;
}
#include <iostream>
bool Entity::Render(sf::RenderWindow* window)
{
	sf::Time elapsed = this->timer.getElapsedTime();
	if (elapsed.asMilliseconds() >= this->fps)
	{
		this->currentFrame += 1;
		if (this->currentFrame > this->endFrame)
		{
			this->currentFrame = this->startFrame;
		}
		this->SetFrame();
		this->timer.restart();
	}
	return true;
}
void Entity::Collision(Entity* entity)
{
}

bool Entity::CheckCollision(Entity* entity)
{
	sf::Vector2f distance = this->getPosition() - entity->getPosition();
	if (distance.x > this->getGlobalBounds().width || distance.y > this->getGlobalBounds().height)
	{
		return false;
	}
	return this->getGlobalBounds().intersects(entity->getGlobalBounds());
}

int Entity::GroupID()
{
	return this->groupId;
}

int Entity::Active()
{
	return this->active;
}

void Entity::Destroy()
{
	this->active = 0;
}

Entity::~Entity()
{
	delete this->texture;
}