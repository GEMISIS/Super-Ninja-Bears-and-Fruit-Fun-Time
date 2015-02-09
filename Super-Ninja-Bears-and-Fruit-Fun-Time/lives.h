#pragma once

#include <SFML/Graphics.hpp>

class Lives : public sf::Text
{
public:
	Lives(sf::Font &font, unsigned int size);
	inline void SetValue(int value)
	{
		this->value = value;
	}
	void RemoveLife();
	void Update();
	int Value();
private:
	int value;
};