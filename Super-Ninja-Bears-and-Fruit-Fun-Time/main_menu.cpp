#include <iostream>
#include "main_menu.h"
#include "main_game.h"
#include "map.h"

void main_menu::Initialize(sf::RenderWindow* window)
{
	this->selected = 0;

	this->font = new sf::Font();
	this->font->loadFromFile("Graphics/font.ttf");

	this->titleTexture = new sf::Texture();
	this->titleTexture->loadFromFile("Graphics/title.png");
	this->title = new sf::Sprite(*this->titleTexture);
	this->title->setOrigin(this->title->getGlobalBounds().width / 2, this->title->getGlobalBounds().height / 2);
	this->title->setPosition(window->getSize().x / 2, window->getSize().y / 8);
	this->title->setColor(sf::Color::Red);

	this->play = new sf::Text("Play", *this->font, 128U);
	this->play->setOrigin(this->play->getGlobalBounds().width / 2, this->play->getGlobalBounds().height / 2);
	this->play->setPosition(window->getSize().x / 2, window->getSize().y / 2);

	this->quit = new sf::Text("Quit", *this->font, 128U);
	this->quit->setOrigin(this->quit->getGlobalBounds().width / 2, this->quit->getGlobalBounds().height / 2);
	this->quit->setPosition(window->getSize().x / 2, window->getSize().y / 2 + this->play->getGlobalBounds().height);

	this->backgroundMusicBuffer = new sf::SoundBuffer();
	this->backgroundMusicBuffer->loadFromFile("Sounds/main menu.wav");
	this->backgroundMusic = new sf::Sound(*this->backgroundMusicBuffer);
	this->backgroundMusic->setLoop(true);
	this->backgroundMusic->play();
}
void main_menu::Update(sf::RenderWindow* window)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && !this->upKey)
	{
		this->selected -= 1;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && !this->downKey)
	{
		this->selected += 1;
	}
	if (this->selected > 1)
	{
		this->selected = 0;
	}
	if (this->selected < 0)
	{
		this->selected = 1;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Return))
	{
		switch (this->selected)
		{
		case 0:
			coreState.SetState(new main_game());
			break;
		case 1:
			quitGame = true;
			break;
		}
	}
	this->upKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up);
	this->downKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down);
}
void main_menu::Render(sf::RenderWindow* window)
{
	this->play->setColor(sf::Color::White);
	this->quit->setColor(sf::Color::White);
	switch (this->selected)
	{
	case 0:
		this->play->setColor(sf::Color::Blue);
		break;
	case 1:
		this->quit->setColor(sf::Color::Blue);
		break;
	}
	window->draw(*this->title);
	window->draw(*this->play);
	window->draw(*this->quit);
}
void main_menu::Destroy(sf::RenderWindow* window)
{
	delete this->font;
	delete this->title;
	delete this->titleTexture;
	delete this->play;
	delete this->quit;

	this->backgroundMusic->stop();

	delete this->backgroundMusic;
	delete this->backgroundMusicBuffer;
}
