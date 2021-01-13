#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Camera.h"
#include "Input.h"
#include "Surface.h"


/* !This has been created as part of the GAME ENGINE unit! */
namespace Engine
{	
	void Camera::onInitialise()
	{
		
	}

	void Camera::setSurface(std::shared_ptr<Surface> _surface)
	{
		m_surface = _surface;
	}

	void Camera::onTick()
	{		
		m_viewMat = glm::lookAt(transform()->getPosition(), transform()->getPosition() + transform()->getFwd(), transform()->getUp());
	}

	std::shared_ptr<Surface> Camera::getSurface()
	{
		return m_surface.lock();
	}
}
