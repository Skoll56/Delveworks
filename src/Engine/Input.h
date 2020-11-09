#ifndef _INPUT_H_
#define _INPUT_H_

#include <SDL2/SDL.h>
namespace Engine
{
	class Input
	{
	public:
		Input();
		bool takeInput(SDL_Event &_event);

		bool m_left, m_right, m_up, m_down, m_asc, m_desc, m_r1, m_r2, m_r, m_f, m_restart;
		int m_xOffset, m_yOffset;
	};
}

#endif