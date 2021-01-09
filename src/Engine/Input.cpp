#include "Input.h"
#include <iostream>
#include "Exception.h"
#include "Core.h"
//#include "Console.h"

namespace Engine
{	
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

	void Mouse::hideCursor(bool _tf)
	{
		m_cursorHidden = _tf; 
		SDL_ShowCursor(!_tf);
	}

	void Mouse::update(std::vector<SDL_Event> _eventList)
	{
		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);
		m_pos.x = mouseX;
		m_pos.y = mouseY;
		
		m_deltaPos = m_pos - m_lastPos;
		if (m_cursorLocked)
		{
			int winX, winY;
			winX = m_IM.lock()->m_windowSize.x / 2;
			winY = m_IM.lock()->m_windowSize.y / 2;
			SDL_WarpMouseInWindow(m_IM.lock()->m_window, winX, winY);			
			m_pos.x = winX;
			m_pos.y = winY;
		}

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
					removeFromList(MouseButton::Left, &m_buttonIsDown);
				}
				else if (_eventList[i].button.button == SDL_BUTTON(SDL_BUTTON_RIGHT))
				{
					m_buttonUp.push_back(MouseButton::Right);
					removeFromList(MouseButton::Right, &m_buttonIsDown);
				}
				else if (_eventList[i].button.button == SDL_BUTTON(SDL_BUTTON_MIDDLE))
				{
					m_buttonUp.push_back(MouseButton::Middle);
					removeFromList(MouseButton::Middle, &m_buttonIsDown);
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
			else if (event.type == SDL_WINDOWEVENT)
			{
				if (event.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					m_core.lock()->onWindowResized(event.window.data1, event.window.data2);
				}
			}

		}
		int x, y;
		SDL_GetWindowSize(m_window, &x, &y);
		m_windowSize.x = x;
		m_windowSize.y = y;

		for (int i = 0; i < m_devices.size(); i++)
		{
			try
			{
				m_devices[i]->update(m_eventList);
			}
			catch (Exception &e)
			{
				Console::output(Console::Error, "InputManager", e.message());
			}
		}

		return quit;
	}

	void InputManager::closeInputDevices()
	{
		for (int i = 0; i < m_devices.size(); i++)
		{
			m_devices[i]->close();
		}
	}

	void Controller::update(std::vector<SDL_Event> _eventList)
	{
		if (!m_init)
		{
			initialise();
		}

		m_leftStick.x = SDL_JoystickGetAxis(m_joystick, 0);
		if (m_leftStick.x < -m_moveThreshold || m_leftStick.x > m_moveThreshold)
		{
			m_leftStick.x = (float)m_leftStick.x / 32767.0f;
		}
		else
		{
			m_leftStick.x = 0.0f;
		}
		m_leftStick.y = SDL_JoystickGetAxis(m_joystick, 1);
		if (m_leftStick.y < -m_moveThreshold || m_leftStick.y > m_moveThreshold)
		{
			m_leftStick.y = (float)m_leftStick.y / 32767.0f;
		}
		else
		{
			m_leftStick.y = 0.0f;
		}
		m_rightStick.x = SDL_JoystickGetAxis(m_joystick, 3);
		if (m_rightStick.x < -m_moveThreshold || m_rightStick.x > m_moveThreshold)
		{
			m_rightStick.x = (float)m_rightStick.x / 32767.0f;
		}
		else
		{
			m_rightStick.x = 0.0f;
		}
		m_rightStick.y = SDL_JoystickGetAxis(m_joystick, 4);
		if (m_rightStick.y < -m_moveThreshold || m_rightStick.y > m_moveThreshold)
		{
			m_rightStick.y = (float)m_rightStick.y / 32767.0f;
		}
		else
		{
			m_rightStick.y = 0.0f;
		}

		m_leftTrigger = (float)SDL_JoystickGetAxis(m_joystick, 2);
		m_leftTrigger /= 32768.0f;
		m_leftTrigger += 1.0f;
		m_leftTrigger /= 2.0f;
		if (m_leftTrigger > 0.01f) { m_leftTrigger += 0.000015f; } //This is because the positive-negative min/max isn't even and that's annoying

		m_rightTrigger = (float)SDL_JoystickGetAxis(m_joystick, 5);
		m_rightTrigger /= 32768.0f;
		m_rightTrigger += 1.0f;
		m_rightTrigger /= 2.0f;
		if (m_rightTrigger > 0.01f) { m_rightTrigger += 0.000015f; } //This is because the positive-negative min/max isn't even and that's annoying

		m_buttonDown.clear();
		m_buttonUp.clear();

		for (int i = 0; i < _eventList.size(); i++)
		{
			if (_eventList[i].type == SDL_CONTROLLERBUTTONDOWN)
			{
				if (_eventList[i].cbutton.button == SDL_CONTROLLER_BUTTON_A)
				{
					m_buttonDown.push_back(PrimaryOne);
					m_buttonIsDown.push_back(PrimaryOne);
				}
				else if (_eventList[i].cbutton.button == SDL_CONTROLLER_BUTTON_B)
				{
					m_buttonDown.push_back(PrimaryTwo);
					m_buttonIsDown.push_back(PrimaryTwo);
				}
				else if (_eventList[i].cbutton.button == SDL_CONTROLLER_BUTTON_X)
				{
					m_buttonDown.push_back(PrimaryThree);
					m_buttonIsDown.push_back(PrimaryThree);
				}
				else if (_eventList[i].cbutton.button == SDL_CONTROLLER_BUTTON_Y)
				{
					m_buttonDown.push_back(PrimaryFour);
					m_buttonIsDown.push_back(PrimaryFour);
				}
				else if (_eventList[i].cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP)
				{
					m_buttonDown.push_back(ArrowUp);
					m_buttonIsDown.push_back(ArrowUp);
				}
				else if (_eventList[i].cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
				{
					m_buttonDown.push_back(ArrowLeft);
					m_buttonIsDown.push_back(ArrowLeft);
				}
				else if (_eventList[i].cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
				{
					m_buttonDown.push_back(ArrowRight);
					m_buttonIsDown.push_back(ArrowRight);
				}
				else if (_eventList[i].cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
				{
					m_buttonDown.push_back(ArrowDown);
					m_buttonIsDown.push_back(ArrowDown);
				}
				else if (_eventList[i].cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSHOULDER)
				{
					m_buttonDown.push_back(LeftBumper);
					m_buttonIsDown.push_back(LeftBumper);
				}
				else if (_eventList[i].cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)
				{
					m_buttonDown.push_back(RightBumper);
					m_buttonIsDown.push_back(RightBumper);
				}
				else if (_eventList[i].cbutton.button == SDL_CONTROLLER_BUTTON_BACK)
				{
					m_buttonDown.push_back(MiddleOne);
					m_buttonIsDown.push_back(MiddleOne);
				}
				else if (_eventList[i].cbutton.button == SDL_CONTROLLER_BUTTON_START)
				{
					m_buttonDown.push_back(MiddleTwo);
					m_buttonIsDown.push_back(MiddleTwo);
				}
				else if (_eventList[i].cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSTICK)
				{
					m_buttonDown.push_back(LeftStick);
					m_buttonIsDown.push_back(LeftStick);
				}
				else if (_eventList[i].cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSTICK)
				{
					m_buttonDown.push_back(RightStick);
					m_buttonIsDown.push_back(RightStick);
				}
			}

			else if (_eventList[i].type == SDL_CONTROLLERBUTTONUP)
			{
				if (_eventList[i].cbutton.button == SDL_CONTROLLER_BUTTON_A)
				{
					m_buttonUp.push_back(PrimaryOne);
					removeFromList(PrimaryOne, &m_buttonIsDown);
				}
				else if (_eventList[i].cbutton.button == SDL_CONTROLLER_BUTTON_B)
				{
					m_buttonUp.push_back(PrimaryTwo);
					removeFromList(PrimaryTwo, &m_buttonIsDown);
				}
				else if (_eventList[i].cbutton.button == SDL_CONTROLLER_BUTTON_X)
				{
					m_buttonUp.push_back(PrimaryThree);
					removeFromList(PrimaryThree, &m_buttonIsDown);
				}
				else if (_eventList[i].cbutton.button == SDL_CONTROLLER_BUTTON_Y)
				{
					m_buttonUp.push_back(PrimaryFour);
					removeFromList(PrimaryFour, &m_buttonIsDown);
				}
				else if (_eventList[i].cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP)
				{
					m_buttonUp.push_back(ArrowUp);
					removeFromList(ArrowUp, &m_buttonIsDown);
				}
				else if (_eventList[i].cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
				{
					m_buttonUp.push_back(ArrowLeft);
					removeFromList(ArrowLeft, &m_buttonIsDown);
				}
				else if (_eventList[i].cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
				{
					m_buttonUp.push_back(ArrowRight);
					removeFromList(ArrowRight, &m_buttonIsDown);
				}
				else if (_eventList[i].cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
				{
					m_buttonUp.push_back(ArrowDown);
					removeFromList(ArrowDown, &m_buttonIsDown);
				}
				else if (_eventList[i].cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSHOULDER)
				{
					m_buttonUp.push_back(LeftBumper);
					removeFromList(LeftBumper, &m_buttonIsDown);
				}
				else if (_eventList[i].cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)
				{
					m_buttonUp.push_back(RightBumper);
					removeFromList(RightBumper, &m_buttonIsDown);
				}
				else if (_eventList[i].cbutton.button == SDL_CONTROLLER_BUTTON_BACK)
				{
					m_buttonUp.push_back(MiddleOne);
					removeFromList(MiddleOne, &m_buttonIsDown);
				}
				else if (_eventList[i].cbutton.button == SDL_CONTROLLER_BUTTON_START)
				{
					m_buttonUp.push_back(MiddleTwo);
					removeFromList(MiddleTwo, &m_buttonIsDown);
				}
				else if (_eventList[i].cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSTICK)
				{
					m_buttonUp.push_back(LeftStick);
					removeFromList(LeftStick, &m_buttonIsDown);
				}
				else if (_eventList[i].cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSTICK)
				{
					m_buttonUp.push_back(RightStick);
					removeFromList(RightStick, &m_buttonIsDown);
				}
			}
		}		
	}

	void Controller::close()
	{
		 SDL_JoystickClose(m_joystick); 
		 SDL_GameControllerClose(m_controller);
	}

	void Controller::initialise()
	{
		m_init = true;
		SDL_JoystickEventState(SDL_ENABLE);
		SDL_GameControllerEventState(SDL_ENABLE);

		if (SDL_NumJoysticks() > 0)
		{
			m_joystick = SDL_JoystickOpen(0);
		}
		else
		{
			throw Exception("No joysticks detected on controller");
		}
		m_controller = SDL_GameControllerOpen(0);
	}

}
