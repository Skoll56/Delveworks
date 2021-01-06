#include "Input.h"
#include <iostream>
#include "Console.h"

namespace Engine
{
	Keyboard::Keyboard()
	{
	};

	bool Keyboard::update() // The input handler / controller
	{
		SDL_Event event = { 0 };
		bool quit = false;		
		m_keyDown.clear();
		m_keyUp.clear();

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}

			else if (event.type == SDL_KEYDOWN)
			{
				m_keyDown.push_back(event.key.keysym.sym);
				m_keyIsDown.push_back(event.key.keysym.sym);
			}

			else if (event.type == SDL_KEYUP)
			{
				for (std::vector<SDL_Keycode>::iterator it = m_keyIsDown.begin(); it != m_keyIsDown.end();)
				{
					if (*it == event.key.keysym.sym)
					{
						it = m_keyIsDown.erase(it);
					}
					else
					{
						it++;
					}
				}
				m_keyUp.push_back(event.key.keysym.sym);
			}
		}

		return quit;
	}


	bool Keyboard::GetKeyIsDown(SDL_Keycode _key)
	{
		for (std::vector<SDL_Keycode>::iterator it = m_keyIsDown.begin(); it != m_keyIsDown.end();)
		{
			if (*it == _key)
			{
				return true;
			}
			else
			{
				it++;
			}
		}

		return false;
	}

	bool Keyboard::GetKeyDown(SDL_Keycode _key)
	{
		for (std::vector<SDL_Keycode>::iterator it = m_keyDown.begin(); it != m_keyDown.end();)
		{
			if (*it == _key)
			{
				return true;
			}
			else
			{
				it++;
			}
		}
		return false;
	}

	bool Keyboard::GetKeyUp(SDL_Keycode _key)
	{
		for (std::vector<SDL_Keycode>::iterator it = m_keyUp.begin(); it != m_keyUp.end();)
		{
			if (*it == _key)
			{
				return true;
			}
			else
			{
				it++;
			}
		}
		return false;
	}

	bool Mouse::update()
	{
		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);
		m_pos.x = mouseX;
		m_pos.y = mouseY;

		SDL_Event event = { 0 };

		while (SDL_PollEvent(&event))
		{
			if (event.button.button == SDL_BUTTON(SDL_BUTTON_LEFT))
			{
				Console::message("Left Click");
			}
		}
	}
}
