#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Camera.h"
#include "Input.h"

namespace Engine
{
	Camera::Camera(glm::vec3 _startPos)
	{
		m_pos = _startPos;
		m_upVec = glm::vec3(0.0f, 1.0f, 0.0f);
		m_yaw = 90.0f;
		m_pitch = 0.0f;
		m_speed = 15.0f;
		m_rotSpeed = 12.0f;
	}

	Camera::~Camera()
	{

	}

	void Camera::update(float _dTime, Input *_input)
	{

		m_yaw += _input->m_xOffset * m_rotSpeed * _dTime;
		m_pitch -= _input->m_yOffset * m_rotSpeed * _dTime;

		if (m_pitch > 89.0f) { m_pitch = 89.0f; }
		else if (m_pitch < -89.0f) { m_pitch = -89.0f; }
		if (m_yaw > 360.0f) { m_yaw = 0.0f; }
		else if (m_yaw < -360.0f) { m_yaw = 0.0f; }

		//Reference LearnOpenGL
		m_fwd.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		m_fwd.y = sin(glm::radians(m_pitch));
		m_fwd.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		m_fwd = glm::normalize(m_fwd);
		m_right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), m_fwd));
	}
}
