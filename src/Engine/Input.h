#ifndef _INPUT_H_
#define _INPUT_H_
#include <vector>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "Console.h"
#include "Exception.h"
//#include "Core.h"

namespace Engine
{
	class InputManager;
	class Core;

	class InputDevice
	{
		friend class InputManager;
	protected:
		virtual void update(std::vector<SDL_Event> _eventList) {};
		virtual void close() {};
		std::weak_ptr<InputManager> m_IM;

		template <typename T>
		bool isContainedIn(T _obj, std::vector<T> _vector)
		{
			for (typename std::vector<T>::iterator it = _vector.begin(); it != _vector.end();)
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

		template <typename T>
		void removeFromList(T _obj, std::vector<T>* _vector)
		{
			for (typename std::vector<T>::iterator it = _vector->begin(); it != _vector->end();)
			{
				if (*it == _obj)
				{
					it = _vector->erase(it);
					break;
				}
				else
				{
					it++;
				}
			}
		}
	};

	class InputManager
	{		
		friend class Mouse;
		friend class Core;
		public:
		bool update();
		void closeInputDevices();


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
					rtn->m_IM = m_self;
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

		glm::vec2 getWindowSize() { return m_windowSize; }
		

		private:		
		SDL_Window* m_window;
		glm::vec2 m_windowSize;
		std::weak_ptr<InputManager> m_self;
		std::weak_ptr<Core> m_core;
		std::vector<SDL_Event> m_eventList;
		std::vector<std::shared_ptr<InputDevice>> m_devices;
	};

	

	class Keyboard : public InputDevice
	{
		friend class InputManager;
	private:
		std::vector<SDL_Keycode> m_keyDown;
		std::vector<SDL_Keycode> m_keyUp;
		std::vector<SDL_Keycode> m_keyIsDown;
		void update(std::vector<SDL_Event> _eventList);

	public:						
		bool GetKeyIsDown(SDL_Keycode _event) { return isContainedIn(_event, m_keyIsDown); }
		bool GetKeyDown(SDL_Keycode _event) { return isContainedIn(_event, m_keyDown); }
		bool GetKeyUp(SDL_Keycode _event) { return isContainedIn(_event, m_keyUp); }
	};


	class Mouse : public InputDevice
	{	
		friend class InputManager;
		public:
		enum MouseButton { Left = 0, Middle = 1, Right = 2 };
		bool getButtonDown(MouseButton _button) { return isContainedIn(_button, m_buttonDown); }
		bool getButtonUp(MouseButton _button) { return isContainedIn(_button, m_buttonUp); }
		bool getButtonIsDown(MouseButton _button) { return isContainedIn(_button, m_buttonIsDown); }
		int getScrollWheel() { return m_scroll; }
		void hideCursor(bool _tf);
		void lockCursor(bool _tf) { m_cursorLocked = _tf; }
		
		glm::vec2 getPosition();
		glm::vec2 getTruePosition() { return m_pos; };
		glm::vec2 getDeltaPos() { return m_deltaPos; }

		private:
		void update(std::vector<SDL_Event> _eventList);
		bool m_cursorHidden;
		bool m_cursorLocked;
		glm::vec2 m_deltaPos;
		glm::vec2 m_pos;
		glm::vec2 m_lastPos;
		int m_scroll;
		std::vector<MouseButton> m_buttonDown;
		std::vector<MouseButton> m_buttonUp;
		std::vector<MouseButton> m_buttonIsDown;
	};

	class Controller : public InputDevice
	{
		friend class InputManager;
	public:
		glm::vec2 getLeftStickPosition() { return m_leftStick; }
		glm::vec2 getRightStickPosition() { return m_rightStick; }
		float getRightTrigger() { return m_rightTrigger; }
		float getLeftTrigger() { return m_leftTrigger; }

		int getThreshold() { return m_moveThreshold; }
		void setThreshold(int _val) { m_moveThreshold = _val; }

		enum ControllerButton
		{
			ArrowUp, ArrowRight, ArrowDown, ArrowLeft, //D-Pad
			LeftBumper, RightBumper, 
			MiddleOne, MiddleTwo, //Select / Start
			PrimaryOne, PrimaryTwo, PrimaryThree, PrimaryFour, //A-B-X-Y
			LeftStick, RightStick
		};

		bool getButtonDown(ControllerButton _button) { return isContainedIn(_button, m_buttonDown); }
		bool getButtonUp(ControllerButton _button) { return isContainedIn(_button, m_buttonUp); }
		bool getButtonIsDown(ControllerButton _button) { return isContainedIn(_button, m_buttonIsDown); }

	private:
		void update(std::vector<SDL_Event> _eventList);
		void close();
		int m_moveThreshold = 6400;
		bool m_init = false;
		SDL_Joystick* m_joystick;
		SDL_GameController* m_controller;
		std::vector<ControllerButton> m_buttonDown;
		std::vector<ControllerButton> m_buttonIsDown;
		std::vector<ControllerButton> m_buttonUp;
		void initialise();
		glm::vec2 m_leftStick;
		glm::vec2 m_rightStick;
		float m_leftTrigger;
		float m_rightTrigger;

	};
}

#endif