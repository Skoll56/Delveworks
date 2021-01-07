#ifndef _INPUT_H_
#define _INPUT_H_
#include <vector>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "Console.h"

namespace Engine
{
	class InputDevice
	{
	public:
		virtual void update(std::vector<SDL_Event> _eventList) {};

	protected:
		template <typename T>
		bool isContainedIn(T _obj, std::vector<T> _vector)
		{
			for (std::vector<T>::iterator it = _vector.begin(); it != _vector.end();)
			{
				if (*it == _obj)
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
	};

	class InputManager
	{
		public:
		bool update();

		template <typename T>
		std::shared_ptr<T> getDevice()
		{
			for (int i = 0; i < m_devices.size(); i++)
			{
				std::shared_ptr<T> rtn = std::dynamic_pointer_cast<T>(m_devices.at(i));
				if (rtn)
				{
					return rtn;
				}
			}
			return nullptr;
		}

		template <typename T>
		std::shared_ptr<T> addDevice()
		{
			try
			{
				std::shared_ptr<T> rtn = std::make_shared<T>();
				if (getDevice<T>())
				{
					Console::output(Console::Error, "InputManager", "Input device has already been added to global InputManager. Only one device of each type can exist.");
				}
				else
				{					
					m_devices.push_back(rtn);					
				}
				return rtn;
			}
			catch(Exception &e)
			{
				Console::output(Console::Error, "InputManager", "Failed to Add Device");
				return nullptr;
			}
		}


		private:		
		std::vector<SDL_Event> m_eventList;
		std::vector<std::shared_ptr<InputDevice>> m_devices;
	};

	

	class Keyboard : public InputDevice
	{
	private:
		std::vector<SDL_Keycode> m_keyDown;
		std::vector<SDL_Keycode> m_keyUp;
		std::vector<SDL_Keycode> m_keyIsDown;

	public:
		Keyboard();
		void update(std::vector<SDL_Event> _eventList);		
		bool GetKeyIsDown(SDL_Keycode _event) { return isContainedIn(_event, m_keyIsDown); }
		bool GetKeyDown(SDL_Keycode _event) { return isContainedIn(_event, m_keyDown); }
		bool GetKeyUp(SDL_Keycode _event) { return isContainedIn(_event, m_keyUp); }
	};


	class Mouse : public InputDevice
	{	
		public:
		enum MouseButton { Left = 0, Middle = 1, Right = 2 };
		bool getButtonDown(MouseButton _button) { return isContainedIn(_button, m_buttonDown); }
		bool getButtonUp(MouseButton _button) { return isContainedIn(_button, m_buttonUp); }
		bool getButtonIsDown(MouseButton _button) { return isContainedIn(_button, m_buttonIsDown); }
		int getScrollWheel() { return m_scroll; }

		void update(std::vector<SDL_Event> _eventList);
		glm::vec2 getPosition() { return m_pos; }
		glm::vec2 getDeltaPos() { return m_deltaPos; }

		private:
		glm::vec2 m_deltaPos;
		glm::vec2 m_pos;
		glm::vec2 m_lastPos;
		int m_scroll;
		std::vector<MouseButton> m_buttonDown;
		std::vector<MouseButton> m_buttonUp;
		std::vector<MouseButton> m_buttonIsDown;
	};
}

#endif