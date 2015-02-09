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
	sf::RenderWindow window(sf::VideoMode(1280, 768), "Super Ninja Bears and Fruit Fun Time");

	coreState.SetWindow(&window);
	coreState.SetState(new main_menu());

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

		coreState.Update();
		coreState.Render();

		window.display();

		if (quitGame)
		{
			window.close();
		}

		Sleep(5);
	}

	return 0;
}
