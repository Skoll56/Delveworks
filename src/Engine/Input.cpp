#include "Input.h"
#include <iostream>
//#include "Console.h"

namespace Engine
{
	Keyboard::Keyboard()
	{
	};

	void Keyboard::update(std::vector<SDL_Event> _eventList) // The input handler / controller
	{		
		m_keyDown.clear();
		m_keyUp.clear();
		
		for (int i = 0; i < _eventList.size(); i++)
		{
			if (_eventList[i].type == SDL_KEYDOWN)
			{
				m_keyDown.push_back(_eventList[i].key.keysym.sym);
				m_keyIsDown.push_back(_eventList[i].key.keysym.sym);
			}

			else if (_eventList[i].type == SDL_KEYUP)
			{
				for (std::vector<SDL_Keycode>::iterator it = m_keyIsDown.begin(); it != m_keyIsDown.end();)
				{
					if (*it == _eventList[i].key.keysym.sym)
					{
						it = m_keyIsDown.erase(it);
					}
					else
					{
						it++;
					}
				}
				m_keyUp.push_back(_eventList[i].key.keysym.sym);
			}
		}
		
	}

	void Mouse::update(std::vector<SDL_Event> _eventList)
	{
		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);
		m_pos.x = mouseX;
		m_pos.y = mouseY;
		
		m_deltaPos = m_pos - m_lastPos;
		m_lastPos = m_pos;


		m_buttonDown.clear();
		m_buttonUp.clear();
		m_scroll = 0;

		for (int i = 0; i < _eventList.size(); i++)
		{
			if (_eventList[i].type == SDL_MOUSEBUTTONDOWN)
			{
				if (_eventList[i].button.button == SDL_BUTTON(SDL_BUTTON_LEFT))
				{
					m_buttonDown.push_back(MouseButton::Left);
					m_buttonIsDown.push_back(MouseButton::Left);
				}
				else if (_eventList[i].button.button == SDL_BUTTON(SDL_BUTTON_RIGHT))
				{
					m_buttonDown.push_back(MouseButton::Right);
					m_buttonIsDown.push_back(MouseButton::Right);
				}
				else if (_eventList[i].button.button == SDL_BUTTON(SDL_BUTTON_MIDDLE))
				{
					m_buttonDown.push_back(MouseButton::Middle);
					m_buttonIsDown.push_back(MouseButton::Middle);
				}				
			}

			else if (_eventList[i].type == SDL_MOUSEBUTTONUP)
			{
				if (_eventList[i].button.button == SDL_BUTTON(SDL_BUTTON_LEFT))
				{
					m_buttonUp.push_back(MouseButton::Left);					
				}
				else if (_eventList[i].button.button == SDL_BUTTON(SDL_BUTTON_RIGHT))
				{
					m_buttonUp.push_back(MouseButton::Right);					
				}
				else if (_eventList[i].button.button == SDL_BUTTON(SDL_BUTTON_MIDDLE))
				{
					m_buttonUp.push_back(MouseButton::Middle);					
				}

				for (std::vector<MouseButton>::iterator it = m_buttonIsDown.begin(); it != m_buttonIsDown.end();)
				{
					if (*it == _eventList[i].key.keysym.sym)
					{
						it = m_buttonIsDown.erase(it);
					}
					else
					{
						it++;
					}
				}
			}
			else if (_eventList[i].type == SDL_MOUSEWHEEL)
			{
				m_scroll = _eventList[i].wheel.y;
			}
		}		
	}

	bool InputManager::update()
	{
		SDL_Event event = { 0 };
		bool quit = false;
		m_eventList.clear();

		while (SDL_PollEvent(&event))
		{
			m_eventList.push_back(event);
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		for (int i = 0; i < m_devices.size(); i++)
		{
			m_devices[i]->update(m_eventList);
		}

		return quit;
	}
}
