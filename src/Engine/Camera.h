#ifndef _CAMERA_H_
#define _CAMERA_H_
#include "Component.h"

namespace Engine
{	
	class Surface;

	/** \brief  A generic Camera component*/
	/** Use this to see things! */
	class Camera : public Component
	{		
		public:
		void onInitialise();

		/** \brief Gives the camera a reference to a Surface that it is rendering to (A DisplayUI)*/
		void setSurface(std::shared_ptr<Surface> _surface);

		/** \brief Gets a reference to a Surface that it is rendering to (A DisplayUI)*/
		std::shared_ptr<Surface> getSurface();

		/** \brief Returns the Camera's view matrix*/
		glm::mat4 getView() { return m_viewMat; }

		/** \brief Returns the Field of View*/
		float getFOV() { return m_FoV; }

		/** \brief Sets the Field of View*/
		void setFOV(float _angle) { m_FoV = _angle; }

		/** \brief Update*/
		void onTick();

		/** \brief A "This" pointer*/
		std::weak_ptr<Camera> m_self;

		private:
		/** \brief The Camera's View Matrix*/
		glm::mat4 m_viewMat;//

		/** \brief The Field of View in degrees*/
		float m_FoV = 45.0f;

		/** \brief A reference to any surface this camera is displaying to*/
		std::weak_ptr<Surface> m_surface; //TODO: This should be an array
	};
}

#endif