#ifndef _INPUT_H_
#define _INPUT_H_
#include <vector>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "Console.h"
#include "Exception.h"


namespace Engine
{
	class InputManager;
	class Core;

	/** \brief InputDevice is the base class for all Input Devices*/
	/** An InputDevice is anything that generates SDL Input events, such as a Controller and Keyboard */
	class InputDevice
	{
		friend class InputManager;
		protected:

		/** \brief Update*/
		virtual void update(std::vector<SDL_Event> _eventList) {};

		/** \brief Close the InputDevice properly.*/
		virtual void close() {};

		/** \brief A reference to the InputManager*/
		std::weak_ptr<InputManager> m_IM;

		/** \brief A Utility function for checking if a variable T is in a Vector*/
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

		/** \brief Removes a variable T from a vector*/
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

	/** \brief Manages all InputDevice objects*/
	/** This is where everything to do with Input happens */
	class InputManager
	{		
		friend class Mouse;
		friend class Core;
		friend class Touchscreen;

		public:

		/** \brief Update*/
		bool update();

		/** \brief Close all input devices properly (this will prevent the device from working)*/
		void closeInputDevices();

		/** \brief Returns an InputDevice of type T*/
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

		/** \brief Adds an InputDevice of Type T to the manager. To get input from a device, this must be done first.*/
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

		/** \brief Gets the current window size*/
		glm::vec2 getWindowSize() { return m_windowSize; }
		

		private:	
		/** \brief A reference to the Window*/
		SDL_Window* m_window;

		/** \brief The Window size*/
		glm::vec2 m_windowSize;

		/** \brief A "this" pointer*/
		std::weak_ptr<InputManager> m_self;

		/** \brief A reference to Core*/
		std::weak_ptr<Core> m_core;

		/** \brief A list of all Input events that happened this frame*/
		std::vector<SDL_Event> m_eventList;

		/** \brief A list of all the InputDevice objects*/
		std::vector<std::shared_ptr<InputDevice>> m_devices;
	};

	
	/** \brief The Keyboard InputDevice*/
	class Keyboard : public InputDevice
	{
		friend class InputManager;

		public:	
		/** \brief Returns true if the Key is currently down*/
		bool GetKeyIsDown(SDL_Keycode _event) { return isContainedIn(_event, m_keyIsDown); }

		/** \brief Returns true if the key was pressed this frame*/
		bool GetKeyDown(SDL_Keycode _event) { return isContainedIn(_event, m_keyDown); }

		/** \brief Returns true if the key was released this frame*/
		bool GetKeyUp(SDL_Keycode _event) { return isContainedIn(_event, m_keyUp); }

		private:
		/** \brief The list of keys pressed this frame*/
		std::vector<SDL_Keycode> m_keyDown;

		/** \brief The list of keys released this frame*/
		std::vector<SDL_Keycode> m_keyUp;

		/** \brief The list of keys that are currently down*/
		std::vector<SDL_Keycode> m_keyIsDown;

		/** \brief Update*/
		void update(std::vector<SDL_Event> _eventList);
	};

	/** \brief The Mouse Input Device*/
	class Mouse : public InputDevice
	{	
		friend class InputManager;
		public:
		/** \brief  An enum that defines the Mouse buttons Left, Middle and Right.*/
		enum MouseButton { Left = 0, Middle = 1, Right = 2 };

		/** \brief Returns true if the button was pressed this frame*/
		bool getButtonDown(MouseButton _button) { return isContainedIn(_button, m_buttonDown); }

		/** \brief Returns true if the button was released this frame*/
		bool getButtonUp(MouseButton _button) { return isContainedIn(_button, m_buttonUp); }

		/** \brief Returns true if the button is currently down*/
		bool getButtonIsDown(MouseButton _button) { return isContainedIn(_button, m_buttonIsDown); }

		/** \brief Returns the value of the scroll wheel where 0 = no movement and positive and negative values represent scrolling up and down.*/
		int getScrollWheel() { return m_scroll; }

		/** \brief Sets whether the cursor is visible or not*/
		void hideCursor(bool _tf);

		/** \brief Locks the Cursor to the centre of the Context*/
		void lockCursor(bool _tf) { m_cursorLocked = _tf; }
		
		/** \brief Returns the position of the Cursor in the Context*/
		glm::vec2 getPosition();

		/** \brief Returns the true position of the Cursor in the Window*/
		glm::vec2 getTruePosition() { return m_pos; };

		/** \brief Returns the amount the Mouse moved this frame*/
		glm::vec2 getDeltaPos() { return m_deltaPos; }

		private:
		/** \brief Update*/
		void update(std::vector<SDL_Event> _eventList);		
		bool m_cursorHidden;
		bool m_cursorLocked;

		/** \brief The difference in position since last frame of the Cursor*/
		glm::vec2 m_deltaPos;

		/** \brief The cursor position*/
		glm::vec2 m_pos;

		/** \brief The last known cursor position as of the last update*/
		glm::vec2 m_lastPos;

		/** \brief The value of the scroll wheel this frame. (Positive, negative or 0)*/
		int m_scroll;

		/** \brief The list of Buttons pressed this frame*/
		std::vector<MouseButton> m_buttonDown;

		/** \brief The list of Buttons released this frame*/
		std::vector<MouseButton> m_buttonUp;

		/** \brief The list of buttons currently down*/
		std::vector<MouseButton> m_buttonIsDown;
	};

	/** \brief The Touch Screen Input Device*/
	class Touchscreen : public InputDevice
	{
		friend class InputManager;
		public:
		

		/** \brief Returns true if the finger was pressed this frame*/
		bool getFingerDown() { return m_fingerDown; }

		/** \brief Returns true if the finger was released this frame*/
		bool getFingerUp() { return m_fingerUp; }

		/** \brief Returns true if the finger is currently down*/
		bool getFingerIsDown() { return m_fingerIsDown; }

		/** \brief Returns the position of the fingerpress in the Context. Returns -1, -1 if not touching. */
		glm::vec2 getPosition();

		/** \brief Returns the true position in the Window. Returns -1, -1 if not touching. */
		glm::vec2 getTruePosition() { return m_pos; };

		/** \brief Returns the amount the finger moved this frame*/
		glm::vec2 getDeltaPos() { return m_deltaPos; }

		private:
		/** \brief Update*/
		void update(std::vector<SDL_Event> _eventList);		

		/** \brief The difference in position since last frame of the Cursor*/
		glm::vec2 m_deltaPos;

		/** \brief The cursor position*/
		glm::vec2 m_pos;

		/** \brief Finger is pressed this frame*/
		bool m_fingerDown;

		/** \brief Finger is released this frame*/
		bool m_fingerUp;

		/** \brief Finger is currently down*/
		bool m_fingerIsDown;
	};

	/** \brief The Controller Input Device*/
	/** The Controller type is generic. It can be PlayStation, Xbox, Nintendo Switch Pro (Although the XY buttons are swapped around) and Steam Controller. */
	class Controller : public InputDevice
	{
		friend class InputManager;
		public:
		/** \brief Returns the Left Joystick's XY position*/
		glm::vec2 getLeftStickPosition() { return m_leftStick; }

		/** \brief Returns the Right Joystick's XY position*/
		glm::vec2 getRightStickPosition() { return m_rightStick; }

		/** \brief Returns how "down" the Right trigger is from 0-1*/
		float getRightTrigger() { return m_rightTrigger; }

		/** \brief Returns how "down" the Left trigger is from 0-1*/
		float getLeftTrigger() { return m_leftTrigger; }

		/** \brief Returns the Movement Threshold for Joysticks*/
		int getThreshold() { return m_moveThreshold; }

		/** \brief Sets the Movement Threshold for Joysticks*/
		void setThreshold(int _val) { m_moveThreshold = _val; }

		/** \brief This enum defines all of the Controller Buttons on a generic Controller*/
		enum ControllerButton
		{
			ArrowUp, ArrowRight, ArrowDown, ArrowLeft, //D-Pad
			LeftBumper, RightBumper, 
			MiddleOne, MiddleTwo, //Select / Start
			PrimaryOne, PrimaryTwo, PrimaryThree, PrimaryFour, //A-B-X-Y
			LeftStick, RightStick
		};
		
		/** \brief Returns true if the button was pressed this frame		*/
		bool getButtonDown(ControllerButton _button) { return isContainedIn(_button, m_buttonDown); }

		/** \brief Returns true if the button was released this frame*/
		bool getButtonUp(ControllerButton _button) { return isContainedIn(_button, m_buttonUp); }

		/** \brief Returns true if the button is currently down*/
		bool getButtonIsDown(ControllerButton _button) { return isContainedIn(_button, m_buttonIsDown); }

		private:
		/** \brief Update*/
		void update(std::vector<SDL_Event> _eventList);

		/** \brief Closes the device*/
		void close();

		/** \brief Initialises the device*/
		void initialise();

		/** \brief The MoveThreshold is the amount a Joystick needs to move before it registers as movement. Joysticks are sometimes overly sensitive.*/
		int m_moveThreshold = 6400;

		/** \brief Tracks whether the device has initialised or not*/
		bool m_init = false;

		/** \brief The Joystick object (Represents both left and right)*/
		SDL_Joystick* m_joystick;

		/** \brief A pointer to the Game Controller object*/
		SDL_GameController* m_controller;

		/** \brief The list of Buttons pressed this frame*/
		std::vector<ControllerButton> m_buttonDown;

		/** \brief The list of buttons currently down*/
		std::vector<ControllerButton> m_buttonIsDown;

		/** \brief The list of Buttons released this frame*/
		std::vector<ControllerButton> m_buttonUp;

		/** \brief The left stick's position XY -1 to 1*/
		glm::vec2 m_leftStick;

		/** \brief The right stick's position XY -1 to 1*/
		glm::vec2 m_rightStick;

		/** \brief The left trigger's position 0-1*/
		float m_leftTrigger;

		/** \brief The right trigger's position 0-1*/
		float m_rightTrigger;
	};
}

#endif