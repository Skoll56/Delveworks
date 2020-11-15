#include "Input.h"
#include <iostream>

namespace Engine
{
	Input::Input()
	{
	};

	bool Input::takeInput(SDL_Event &_event) // The input handler / controller
	{
		bool quit = false;		
		m_keyDown.clear();
		m_keyUp.clear();

		while (SDL_PollEvent(&_event))
		{
			if (_event.type == SDL_QUIT)
			{
				quit = true;
			}

			else if (_event.type == SDL_KEYDOWN)
			{
				if (_event.key.keysym.sym == SDLK_ESCAPE)
				{
					quit = true;
				}
				else
				{
					m_keyDown.push_back(_event.key.keysym.sym);
					m_keyIsDown.push_back(_event.key.keysym.sym);
				}				
			}

			else if (_event.type == SDL_KEYUP)
			{
				for (std::vector<SDL_Keycode>::iterator it = m_keyIsDown.begin(); it != m_keyIsDown.end();)
				{
					if (*it == _event.key.keysym.sym)
					{
						it = m_keyIsDown.erase(it);
					}
					else
					{
						it++;
					}
				}
				m_keyUp.push_back(_event.key.keysym.sym);
			}
		}

		return quit;
	}


	bool Input::GetKeyIsDown(SDL_Keycode _key)
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

	bool Input::GetKeyDown(SDL_Keycode _key)
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

	bool Input::GetKeyUp(SDL_Keycode _key)
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
}
