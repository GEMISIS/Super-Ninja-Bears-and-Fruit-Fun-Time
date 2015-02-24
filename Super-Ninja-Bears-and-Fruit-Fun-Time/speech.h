#pragma once

#include <SFML/Graphics.hpp>

class Speech : public sf::Text, public sf::Sprite
{
public:
	bool speaking, spaceKey;

	Speech(sf::Font &font, unsigned int size, sf::RenderWindow* window);
	void SetText(std::string characterName, std::string text);
	void Render();
	~Speech();
private:
	std::string characterName;
	std::string text;
	sf::Texture* texture;
	sf::RenderWindow* window;

	void SetString(std::string text);
};


