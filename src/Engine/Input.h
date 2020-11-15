#ifndef _INPUT_H_
#define _INPUT_H_
#include <vector>
#include <SDL2/SDL.h>
namespace Engine
{
	class Input
	{
	private:
		std::vector<SDL_Keycode> m_keyDown;
		std::vector<SDL_Keycode> m_keyUp;
		std::vector<SDL_Keycode> m_keyIsDown;

	public:
		Input();
		bool takeInput(SDL_Event &_event);
		//bool m_left, m_right, m_up, m_down, m_asc, m_desc, m_r1, m_r2, m_r, m_f, m_restart;
		bool GetKeyIsDown(SDL_Keycode _event);
		bool GetKeyDown(SDL_Keycode _event);
		bool GetKeyUp(SDL_Keycode _event);
		


		int m_xOffset, m_yOffset;
	};
}

#endif