#ifndef _CAMERA_H_
#define _CAMERA_H_
#include "Component.h"

namespace Engine
{
	class Keyboard;	
	class Camera : public Component
	{		
		private:		
		float m_rotSpeed;
		glm::mat4 m_viewMat;

		public:				
		void onInitialise();
		glm::mat4 getView() { return m_viewMat; }
		void update(float _dTime);
	};
}

#endif