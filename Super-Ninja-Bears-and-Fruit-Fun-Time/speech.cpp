#include "speech.h"

#define LINES_WIDTH 30
#define MAX_LINES 3

Speech::Speech(sf::Font &font, unsigned int size, sf::RenderWindow* window) : sf::Text("", font, size)
{
	this->texture = new sf::Texture();
	this->texture->loadFromFile("Graphics/dialog_box.png");
	this->setTexture(*this->texture);
	sf::Sprite::setPosition(0, window->getSize().y - sf::Text::getGlobalBounds().height);
	sf::Text::setPosition(5.0f, window->getSize().y - sf::Text::getGlobalBounds().height);
	this->window = window;
	this->spaceKey = false;
	this->speaking = false;
}

void Speech::SetText(std::string characterName, std::string text)
{
	this->speaking = true;

	this->characterName = characterName;

	text = characterName + ": " + text;
	if (text.length() > 32)
	{
		int lines = 0;
		int offset = 0;
		for (int i = 0; i < LINES_WIDTH * MAX_LINES && i < text.length(); i += 1)
		{
			if (text[i] == ' ')
			{
				if (i % LINES_WIDTH < lines && lines < MAX_LINES)
				{
					text[i] = '\n';
					lines += 1;
				}
				offset = i;
			}
		}
		if (text.length() < LINES_WIDTH * MAX_LINES)
		{
			for (int i = lines; i < MAX_LINES; i += 1)
			{
				text += "\n";
			}
			this->SetString(text);
			this->text = "";
		}
		else
		{
			this->SetString(text.substr(0, offset));
			this->text = text.substr(offset, text.length());
		}
	}
	else
	{
		for (int i = 0; i < MAX_LINES; i += 1)
		{
			text += "\n";
		}
		this->SetString(text);
		this->text = "";
	}
}

void Speech::SetString(std::string text)
{
	this->setString(text);
	float scaleWidth = sf::Text::getGlobalBounds().width / sf::Sprite::getTextureRect().width + 0.2f;
	float scaleHeight = sf::Text::getGlobalBounds().height / sf::Sprite::getTextureRect().height + 0.2f;

	sf::Sprite::setScale(scaleWidth, scaleHeight);

	sf::Sprite::setPosition(this->window->getSize().x / 2 - sf::Text::getGlobalBounds().width / 2, this->window->getSize().y - sf::Text::getGlobalBounds().height - 64);
	sf::Text::setPosition(this->window->getSize().x / 2 - sf::Text::getGlobalBounds().width / 2 + 5.0f, this->window->getSize().y - sf::Text::getGlobalBounds().height - 64);
}

void Speech::Render()
{
	if (this->speaking)
	{
		bool spaceKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
		if (spaceKey && !this->spaceKey)
		{
			while (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space));
			if (this->text.length() > 0)
			{
				this->SetText(this->characterName, this->text);
			}
			else
			{
				this->speaking = false;
			}
		}
		this->window->draw((sf::Sprite)*this);
		this->window->draw((sf::Text)*this);
		this->spaceKey = spaceKey;
	}
}

Speech::~Speech()
{
	delete this->texture;
}