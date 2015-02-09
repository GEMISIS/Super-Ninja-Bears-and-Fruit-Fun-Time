#pragma once

#include <SFML/Audio.hpp>

#include "game_state.h"

class main_menu : public tiny_state
{
public:
	void Initialize(sf::RenderWindow* window);
	void Update(sf::RenderWindow* window);
	void Render(sf::RenderWindow* window);
	void Destroy(sf::RenderWindow* window);
private:
	sf::Font* font;
	sf::Texture* titleTexture;
	sf::Sprite* title;
	sf::Text* play;
	sf::Text* quit;

	int selected;

	bool upKey, downKey;

	sf::SoundBuffer* backgroundMusicBuffer;
	sf::Sound* backgroundMusic;
};