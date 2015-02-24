#include <Windows.h>

#include "game_state.h"
#include "main_menu.h"

game_state coreState; 
bool quitGame = false;

#ifdef _DEBUG
int main()
#else
int WINAPI WinMain(
	_In_  HINSTANCE hInstance,
	_In_opt_  HINSTANCE hPrevInstance,
	_In_  LPSTR lpCmdLine,
	_In_  int nCmdShow
	)
#endif
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Super Ninja Bears and Fruit Fun Time", sf::Style::Close);

	sf::Clock updateTimer;

	window.setVerticalSyncEnabled(true);

	coreState.SetWindow(&window);
	coreState.SetState(new main_menu());

#ifdef _DEBUG
	sf::Clock fpsTimer;
	sf::Font fpsFont;
	fpsFont.loadFromFile("Graphics/font.ttf");
	sf::Text fpsText("FPS: 0", fpsFont);
	fpsText.setColor(sf::Color::Red);
	fpsText.setPosition(0, window.getSize().y - fpsText.getGlobalBounds().height - 12);
#endif

	// run the program as long as the window is open
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color::Black);

		if (updateTimer.getElapsedTime().asMicroseconds() >= 0)
		{
			if (window.hasFocus())
			{
				coreState.Update();
				coreState.Render();
#ifdef _DEBUG
				window.draw(fpsText);
#endif
				window.display();
			}
			updateTimer.restart();
#ifdef _DEBUG
			fpsText.setString("FPS: " + std::to_string(1000000.0f / fpsTimer.restart().asMicroseconds()));
#endif
		}

		if (quitGame)
		{
			window.close();
		}
	}

	return 0;
}
