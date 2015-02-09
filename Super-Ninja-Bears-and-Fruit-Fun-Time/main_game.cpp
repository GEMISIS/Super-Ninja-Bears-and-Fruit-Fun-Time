#include "main_game.h"
#include "main_menu.h"
#include "game_over.h"
#include "win_screen.h"

#include "main_guy.h"
#include "enemy.h"

bool gameOver = false;
SaveSystem saveSystem;

void main_game::Initialize(sf::RenderWindow* window)
{
	this->font = new sf::Font();
	this->font->loadFromFile("Graphics/font.ttf");
	this->score = new Score(*font, 64U);
	this->lives = new Lives(*font, 64U);
	this->lives->setPosition(window->getSize().x - this->lives->getGlobalBounds().width - 15, 0);
	this->score->setColor(sf::Color::Black);
	this->lives->setColor(sf::Color::Black);
	this->speech = new Speech(*font, 32U, window);

	this->pausedText = new sf::Text("Paused\nPress Escape to Quit", *font, 64U);
	this->pausedText->setOrigin(this->pausedText->getGlobalBounds().width / 2, this->pausedText->getGlobalBounds().height / 2);
	this->pausedText->setPosition(window->getSize().x / 2, window->getSize().y / 2);
	this->pausedText->setColor(sf::Color::Black);

	this->paused = false;
	this->enterKey = true;

	manager = new EntityManager();

	map = new Map(manager);
	map->Load(saveSystem.currentMap, "uncolored_hills.png", this->speech);

	this->manager->Add("main_guy", new main_guy(window, manager, map, saveSystem.x, saveSystem.y));

	this->backgroundMusicBuffer = new sf::SoundBuffer();
	this->backgroundMusicBuffer->loadFromFile("Sounds/first area.wav");
	this->backgroundMusic = new sf::Sound(*this->backgroundMusicBuffer);
	this->backgroundMusic->setLoop(true);
	this->backgroundMusic->play();
}
void main_game::Update(sf::RenderWindow* window)
{
	this->lives->SetValue(((main_guy*)this->manager->Get("main_guy"))->health);
	this->score->SetValue(saveSystem.GetScore());
	if (this->paused)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Return) && !this->enterKey)
		{
			this->paused = false;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
		{
			coreState.SetState(new main_menu());
			return;
		}
	}
	else if (this->speech->speaking)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Return) && !this->enterKey)
		{
			this->speech->speaking = false;
		}
	}
	else
	{
		if (!this->manager->Update(window))
		{
			return;
		}
		this->score->Update();
		this->lives->Update();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Return) && !this->enterKey)
		{
			this->paused = true;
		}
	}
	if (gameOver || this->lives->Value() <= 0)
	{
		gameOver = false;
		coreState.SetState(new game_over());
		return;
	}
	this->enterKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Return);
	if (!this->paused)
	{
		this->map->Update(window);
	}
}
void main_game::Render(sf::RenderWindow* window)
{
	map->Render(window);
	this->manager->Render(window, !this->paused);
	window->draw(*this->score);
	window->draw(*this->lives);
	this->speech->Render();

	if (this->paused)
	{
		window->draw(*this->pausedText);
	}
}
void main_game::Destroy(sf::RenderWindow* window)
{
	delete this->score;
	delete this->lives;
	delete this->pausedText;
	delete this->font;

	delete this->speech;

	delete this->manager;
	delete this->map;

	this->backgroundMusic->stop();

	delete this->backgroundMusic;
	delete this->backgroundMusicBuffer;
}
