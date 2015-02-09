#include "main_game.h"
#include "main_guy.h"
#include "bullet.h"
#include "win_screen.h"
#include "player_bullet.h"
#include "bronze_coin.h"
#include "silver_coin.h"

main_guy::main_guy(sf::RenderWindow* window, EntityManager* entityManager, Map* map, float x, float y)
{
	this->Load("sprites/ninja.png", sf::IntRect(0, 0, 88, 88));
	this->setPosition(x, y);
	this->map = map;
	this->speed = 2.0f;
	this->jump = 0;
	this->run = 0;
	this->attack = 0;
	this->groupId = 1;
	this->entityManager = entityManager;
	Entity::scroll = sf::Vector2f(0, 0);
	this->fps = 16 * 5;
	this->yOffset = -8;

	this->Flip(saveSystem.flip);
	if (saveSystem.flip)
	{
		this->direction = 180;
	}

	this->health = saveSystem.health;

	while (this->getPosition().x + this->getGlobalBounds().width + 512 > window->getSize().x && Entity::scroll.x < this->map->getGlobalBounds().width - window->getSize().x)
	{
		this->map->move(-abs(this->speed), 0);
		this->move(-abs(this->speed), 0);
		Entity::scroll.x += abs(this->speed);
	}
	while (this->getPosition().x - 512 < 0 && Entity::scroll.x > 0)
	{
		this->map->move(abs(this->speed), 0);
		this->move(abs(this->speed), 0);
		Entity::scroll.x -= abs(this->speed);
	}
	while (this->getPosition().y + this->getGlobalBounds().height + 256 > window->getSize().y && Entity::scroll.y < this->map->getGlobalBounds().height - window->getSize().y)
	{
		this->map->move(0, -abs(this->speed));
		this->move(0, -abs(this->speed));
		Entity::scroll.y += abs(this->speed);
	}
	while (this->getPosition().y - 256 < 0 && Entity::scroll.y > 0)
	{
		this->map->move(0, abs(this->speed));
		this->move(0, abs(this->speed));
		Entity::scroll.y -= abs(this->speed);
	}
}

void main_guy::Swimming(sf::RenderWindow* window)
{
	this->jump = 0;
	this->velocity.x = (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) - sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) * this->speed;
	this->velocity.y = (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) - sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) * this->speed;

	if (this->map->CheckCollision(this, LEFT).group == GROUND)
	{
		this->move(-abs(this->speed), 0);
	}
	if (this->map->CheckCollision(this, RIGHT).group == GROUND)
	{
		this->move(abs(this->speed), 0);
	}
	if (this->map->CheckCollision(this, UP).group == GROUND)
	{
		this->move(0, abs(this->speed));
	}
	if (this->map->CheckCollision(this, DOWN).group == GROUND)
	{
		this->move(0, -abs(this->speed));
	}
}

void main_guy::Ground(sf::RenderWindow* window)
{
	this->velocity.x = (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) - sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) * this->speed;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
	{
		this->SetFrame(0);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
	{
		this->fps = 16 * 5;
		this->Animate(1, 4);
	}
	else if (this->attack == 0)
	{
		this->SetFrame(0);
	}

	if (this->velocity.y < 3.0f)
	{
		this->velocity.y += 0.1f;
	}

	if (this->map->CheckCollision(this, LEFT).group == GROUND)
	{
		this->move(-abs(this->speed), 0);
	}
	if (this->map->CheckCollision(this, RIGHT).group == GROUND)
	{
		this->move(abs(this->speed), 0);
	}
	if (this->map->CheckCollision(this, UP).group == GROUND)
	{
		this->move(0, abs(this->speed));
		this->velocity.y = 0.0f;
	}
	if (this->map->CheckCollision(this, DOWN).group == GROUND)
	{
		this->velocity.y = 0.0f;
		this->jump = 0;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && this->jump == 0)
	{
		this->velocity.y = -3.0f;
		this->jump = 1;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && this->jump == 1 && this->velocity.y > -5.0f)
	{
		this->velocity.y -= 0.25f;
	}
	else if (this->velocity.y <= -5.0f)
	{
		this->jump = 2;
	}

	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift)) && (this->run == 0 && this->speed < 4.0f))
	{
		this->speed += 0.01f;
	}
	else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift)) && (this->run == 0 && this->speed >= 4.0f))
	{
		this->run = 1;
	}
	else if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift)) && this->speed > 2.0f)
	{
		this->speed -= 0.02f;
		this->run = 0;
	}
}

bool main_guy::Update(sf::RenderWindow* window)
{
	if (this->map->CheckCollision(this, NONE).group == WATER)
	{
		this->Swimming(window);
	}
	else
	{
		this->Ground(window);
	}

	if (this->map->CheckCollision(this, NONE).group == EXIT)
	{
		if (this->spaceKey)
		{
			saveSystem.addAreaToFinished();
			saveSystem.currentAreaEntry = this->map->CheckCollision(this, NONE).nextAreaEntry;
			saveSystem.currentMap = this->map->CheckCollision(this, NONE).nextArea;
			saveSystem.flip = this->flippedH;
			saveSystem.Save();
			coreState.SetState(new main_game());
			return false;
		}
	}
	if (this->map->CheckCollision(this, NONE).group == FINISHED)
	{
		if (this->spaceKey)
		{
			saveSystem.reset();
			saveSystem.Save();
			coreState.SetState(new win_screen());
			return false;
		}
	}

	if (this->map->CheckCollision(this, DOWN).group == LAVA)
	{
		if (this->jump == 0)
		{
			this->damageKnockback = (this->flippedH) ? 4 : -4;
		}
		this->velocity.x = this->damageKnockback;
		this->velocity.y = -3;
		this->jump = 1;
		this->health -= 1;
	}

	this->velocity.x += this->damageKnockback;

	if (abs(this->velocity.x) < 1 && 
		(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)))
	{
		damageKnockback = 0;
	}

	if (this->damageKnockback > 0)
	{
		this->damageKnockback -= 0.05f;
	}
	else if (this->damageKnockback < 0)
	{
		this->damageKnockback += 0.05f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
	{
		this->Flip(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left));
	}

	if (this->velocity.x > 0)
	{
		this->direction = 0.0f;
	}
	else if (this->velocity.x < 0)
	{
		this->direction = 180.0f;
	}

	if (this->getPosition().x + this->getGlobalBounds().width + 512 > window->getSize().x && Entity::scroll.x < this->map->getGlobalBounds().width - window->getSize().x)
	{
		this->map->move(-abs(this->speed), 0);
		this->move(-abs(this->speed), 0);
		Entity::scroll.x += abs(this->speed);
	}
	if (this->getPosition().x - 512 < 0 && Entity::scroll.x > 0)
	{
		this->map->move(abs(this->speed), 0);
		this->move(abs(this->speed), 0);
		Entity::scroll.x -= abs(this->speed);
	}
	if (this->getPosition().y + this->getGlobalBounds().height + 256 > window->getSize().y && Entity::scroll.y < this->map->getGlobalBounds().height - window->getSize().y)
	{
		this->map->move(0, -abs(this->speed + this->velocity.y));
		this->move(0, -abs(this->speed + this->velocity.y));
		Entity::scroll.y += abs(this->speed + this->velocity.y);
	}
	if (this->getPosition().y - 256 < 0 && Entity::scroll.y > 0)
	{
		this->map->move(0, abs(this->speed));
		this->move(0, abs(this->speed));
		Entity::scroll.y -= abs(this->speed);
	}

	/*
	if (this->getPosition().x <= 0)
	{
	saveSystem.currentMap = map->leftArea;
	saveSystem.x = window->getSize().x - this->getGlobalBounds().width - 32;
	saveSystem.y = this->getPosition().y;
	coreState.SetState(new main_game());
	return false;
	}
	if (this->getPosition().x + this->getGlobalBounds().width >= window->getSize().x)
	{
	saveSystem.currentMap = map->rightArea;
	saveSystem.x = 32;
	saveSystem.y = this->getPosition().y;
	coreState.SetState(new main_game());
	return false;
	}
	*/

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && !this->spaceKey)
	{
		this->entityManager->Add("bullet", new Bullet(this->map, this->getPosition().x + Entity::scroll.x, this->getPosition().y + this->getGlobalBounds().height / 8 + Entity::scroll.y, direction, 64));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X) && !this->xKey)
	{
		this->entityManager->Add("pBullet", new PlayerBullet(this->map, this->getPosition().x + Entity::scroll.x, this->getPosition().y + this->getGlobalBounds().height / 4 + Entity::scroll.y, direction, 800));
		this->SetFrame(6);
		this->SetFrame();
	}

	this->spaceKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
	this->xKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X);

	Entity::Update(window);
	return true;
}

void main_guy::Collision(Entity* entity)
{
	switch (entity->GroupID())
	{
	case 5:
		this->damageKnockback = (this->flippedH) ? 4 : -4;
		this->velocity.x = 0;
		this->velocity.y = -3.0f;
		this->jump = 0;
		this->health -= 1;

		entity->velocity.x *= -1;
		entity->flippedH = !entity->flippedH;
		break;
	case 6:
		entity->Destroy();
		this->health += 1;
		break;
	case 7:
		((bronze_coin*)entity)->Destroy();
		saveSystem.IncrementScore(1);
		break;
	case 8:
		((silver_coin*)entity)->Destroy();
		saveSystem.IncrementScore(2);
		break;
	}
}

