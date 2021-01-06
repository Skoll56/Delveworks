#ifndef _INPUT_H_
#define _INPUT_H_
#include <vector>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

namespace Engine
{
	class Input
	{

	};

	class Keyboard : public Input
	{
	private:
		std::vector<SDL_Keycode> m_keyDown;
		std::vector<SDL_Keycode> m_keyUp;
		std::vector<SDL_Keycode> m_keyIsDown;

	public:
		Keyboard();
		bool update();		
		bool GetKeyIsDown(SDL_Keycode _event);
		bool GetKeyDown(SDL_Keycode _event);
		bool GetKeyUp(SDL_Keycode _event);
	};


	class Mouse : public Input
	{
		glm::vec2 m_pos;
		int m_xOffset, m_yOffset;

		public:
		bool update();
		glm::vec2 getPosition() { return m_pos; }
		
	};
}

#endif