#ifndef _CAMERA_H_
#define _CAMERA_H_
#include "Component.h"

namespace Engine
{
	class Input;	
	class Camera : public Component
	{		
		private:		
		float m_rotSpeed;

		public:				
		void onInitialise();
		void update(float _dTime, Input *_input);
	};
}

#endif