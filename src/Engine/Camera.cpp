#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Camera.h"
#include "Input.h"
#define OUTPUT(vec) std::cout << vec.x << " " << vec.y << " " << vec.z <<std::endl;

/* !This has been created as part of the GAME ENGINE unit! */
namespace Engine
{	
	void Camera::onInitialise()
	{
		m_rotSpeed = 12.0f;
	}

	void Camera::update(float _dTime, std::shared_ptr<Input> _input)
	{

		transform()->rotate(glm::vec3(0.0f, 1.0f, 0.0f), _input->m_xOffset * m_rotSpeed * _dTime);
		transform()->rotate(glm::vec3(1.0f, 0.0f, 0.0f), _input->m_yOffset * m_rotSpeed * _dTime);

		if (transform()->m_eulerAngles.x > 89.0f) { transform()->m_eulerAngles.x = 89.0f; }
		else if (transform()->m_eulerAngles.x < -89.0f) { transform()->m_eulerAngles.x = -89.0f; }
		if (transform()->m_eulerAngles.y > 360.0f) { transform()->m_eulerAngles.y = 0.0f; }
		else if (transform()->m_eulerAngles.y < -360.0f) { transform()->m_eulerAngles.y = 0.0f; }

		if (input()->GetKeyIsDown(SDLK_w))
		{
			transform()->m_position += transform()->getFwd() * 0.16f;
		}
		if (input()->GetKeyIsDown(SDLK_s))
		{
			transform()->m_position -= transform()->getFwd() * 0.16f;
		}
		if (input()->GetKeyIsDown(SDLK_d))
		{
			transform()->m_position -= transform()->getRight() * 0.16f;
		}
		if (input()->GetKeyIsDown(SDLK_a))
		{
			transform()->m_position += transform()->getRight() * 0.16f;
		}
		
		OUTPUT(transform()->m_eulerAngles);
	}
}
