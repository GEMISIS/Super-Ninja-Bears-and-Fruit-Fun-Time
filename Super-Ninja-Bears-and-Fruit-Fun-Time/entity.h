#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class Entity : public sf::Sprite
{
public:
	int fps;
	sf::Vector2f velocity;
	int xOffset, yOffset;
	static sf::Vector2f scroll;

	Entity();

	void Load(std::string filename, const sf::IntRect& area = sf::IntRect());

	void SetFrameSize(sf::IntRect size);

	virtual bool Update(sf::RenderWindow* window);

	virtual bool Render(sf::RenderWindow* window);

	bool CheckCollision(Entity* entity);

	virtual void Collision(Entity* entity);

	inline void SetFrame()
	{
		int frameX = this->currentFrame * this->frameSize.width;
		int frameY = 0;
		while (frameX >= this->texture->getSize().x)
		{
			frameX -= this->texture->getSize().x;
			frameY += this->frameSize.height;
		}
		this->frameSize.left = frameX;
		this->frameSize.top = frameY;

		int left = (this->flippedH) ? (this->frameSize.left + this->xOffset) + this->frameSize.width : (this->frameSize.left + this->xOffset);
		int top = (this->flippedV) ? (this->frameSize.top + this->yOffset) + this->frameSize.height : (this->frameSize.top + this->yOffset);
		int width = (this->flippedH) ? -this->frameSize.width : this->frameSize.width;
		int height = (this->flippedV) ? -this->frameSize.height : this->frameSize.height;
		this->setTextureRect(sf::IntRect(left, top, width, height));
		this->timer.restart();
	}

	inline void Animate(int start, int end)
	{
		this->startFrame = start;
		this->endFrame = end;
	}

	inline void SetFrame(int frame)
	{
		this->startFrame = this->endFrame = this->currentFrame = frame;
	}

	inline void Flip(bool horizontal = false, bool vertical = false)
	{
		this->flippedH = horizontal;
		this->flippedV = vertical;
	}

	int GroupID();

	int Active();

	virtual void Destroy();

	~Entity();
	bool flippedH, flippedV;
protected:
	int active;
	int groupId;
	int currentFrame;
private:
	int startFrame, endFrame;
	sf::Clock timer;
	sf::IntRect frameSize;
	sf::Texture* texture;
};

