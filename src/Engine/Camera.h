#ifndef _CAMERA_H_
#define _CAMERA_H_
#include "Component.h"

namespace Engine
{	
	class Surface;
	class Camera : public Component
	{		
		//friend class Surface;

		private:		
		float m_rotSpeed;
		glm::mat4 m_viewMat;
		float m_FoV = 45.0f;
		std::weak_ptr<Surface> m_surface;

		public:				
		void onInitialise();
		void setSurface(std::shared_ptr<Surface> _surface);
		std::shared_ptr<Surface> getSurface();
		glm::mat4 getView() { return m_viewMat; }
		float getFOV() { return m_FoV; }
		void setFOV(float _angle) { m_FoV = _angle; }
		void onTick();
	};
}

#endif