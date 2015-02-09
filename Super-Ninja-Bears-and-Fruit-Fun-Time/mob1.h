#pragma once

#include "enemy.h"

class Mob1 : public enemy
{
public:
	Mob1(Map* map, float x, float y) : enemy(map, "sprites/ninja.png", x, y, sf::IntRect(0, 0, 88, 88))
	{
		this->velocity = sf::Vector2f(-1, 0);
		this->Animate(0, 4);
		this->yOffset = -8;
		this->fps = 16 * 5;
	}

	bool Update(sf::RenderWindow* window)
	{
		this->setPosition(this->position.x - Entity::scroll.x, this->position.y - Entity::scroll.y);
		if (this->health > 0)
		{
			int tileL = this->map->CheckCollision(this, Direction::RIGHT).group;
			int tileR = this->map->CheckCollision(this, Direction::LEFT).group;
			int tileBL = this->map->CheckCollision(this, Direction::BOTTOM_RIGHT).group;
			int tileBR = this->map->CheckCollision(this, Direction::BOTTOM_LEFT).group;
			if ((tileL == GROUND || tileBL != GROUND || this->position.x < 0) && this->velocity.x < 0)
			{
				this->velocity.x *= -1;
			}
			if ((tileR == GROUND || tileBR != GROUND || this->position.x + this->getGlobalBounds().width > this->map->getGlobalBounds().width) && this->velocity.x > 0)
			{
				this->velocity.x *= -1;
			}
			this->Flip(this->velocity.x < 0);
			this->offset(this->velocity.x, this->velocity.y);
		}
		else  if (this->health <= 0 && this->currentFrame < 15)
		{
			this->SetFrame(15);
			this->Animate(16, 17);
			this->fps = 16 * 8;
		}
		if (this->currentFrame >= 17)
		{
			this->Destroy();
			saveSystem.IncrementScore(1);
		}
		return true;
	}
};