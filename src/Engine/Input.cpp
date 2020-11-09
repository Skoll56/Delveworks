#include "Input.h"
#include <iostream>

namespace Engine
{
	Input::Input()
	{

		m_left = false;
		m_right = false;
		m_up = false;
		m_down = false;
		m_asc = false;
		m_desc = false;
		m_r1 = false;
		m_r2 = false;
		m_r = false;
		m_f = false;
		m_restart = false;
	};

	bool Input::takeInput(SDL_Event &_event) // The input handler / controller
	{
		bool quit = false;		
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

				if (_event.key.keysym.sym == SDLK_a)
				{
					m_left = true;
				}

				if (_event.key.keysym.sym == SDLK_p)
				{
					m_restart = true;
				}

				if (_event.key.keysym.sym == SDLK_d)
				{
					m_right = true;
				}
				if (_event.key.keysym.sym == SDLK_w)
				{
					m_up = true;
				}
				if (_event.key.keysym.sym == SDLK_s)
				{
					m_down = true;
				}

				if (_event.key.keysym.sym == SDLK_SPACE)
				{
					m_asc = true;
				}

				if (_event.key.keysym.sym == SDLK_LSHIFT)
				{
					m_desc = true;
				}

				if (_event.key.keysym.sym == SDLK_q)
				{
					m_r2 = true; //Key Q
				}
				if (_event.key.keysym.sym == SDLK_e)
				{
					m_r1 = true; //Key E
				}
				if (_event.key.keysym.sym == SDLK_r)
				{
					m_r = true;
				}
				if (_event.key.keysym.sym == SDLK_f)
				{
					m_f = true;
				}
			}

			else if (_event.type == SDL_KEYUP)
			{
				if (_event.key.keysym.sym == SDLK_a)
				{
					m_left = false;
				}
				if (_event.key.keysym.sym == SDLK_d)
				{
					m_right = false;
				}
				if (_event.key.keysym.sym == SDLK_w)
				{
					m_up = false;
				}
				if (_event.key.keysym.sym == SDLK_s)
				{
					m_down = false;
				}

				if (_event.key.keysym.sym == SDLK_SPACE)
				{
					m_asc = false;
				}

				if (_event.key.keysym.sym == SDLK_LSHIFT)
				{
					m_desc = false;
				}
				if (_event.key.keysym.sym == SDLK_q)
				{
					m_r2 = false;
				}
				if (_event.key.keysym.sym == SDLK_e)
				{
					m_r1 = false;
				}
				if (_event.key.keysym.sym == SDLK_r)
				{
					m_r = false;
				}
				if (_event.key.keysym.sym == SDLK_f)
				{
					m_f = false;
				}
			}


		}

		return quit;
	}
}
