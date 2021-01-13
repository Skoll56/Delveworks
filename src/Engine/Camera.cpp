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

	void Camera::addSurface(std::shared_ptr<Surface> _surface)
	{
		m_surface.push_back(_surface);
	}

	void Camera::onTick()
	{		
		m_viewMat = glm::lookAt(transform()->getPosition(), transform()->getPosition() + transform()->getFwd(), transform()->getUp());
		for (std::vector<std::weak_ptr<Surface>>::iterator it = m_surface.begin(); it != m_surface.end();)
		{
			if (!(*it).lock())
			{
				it = m_surface.erase(it); //Remove any surfaces which were deleted
			}
			else
			{
				it++;
			}
		}
	}

	std::shared_ptr<Surface> Camera::getSurface(int _i)
	{
		if (_i < m_surface.size())
		{
			return m_surface[_i].lock();
		}
	}
}
