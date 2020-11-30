#include <glm/glm.hpp>
#include "Component.h"
#include "Light.h"
#include "Entity.h"
#include "Shader.h"

namespace Engine
{
	void DirLight::setValues(glm::vec3 _diffuse, float _specular, glm::vec3 _ambient)
	{
		m_diffuse = _diffuse;
		m_specIntens = _specular;
		m_ambient = _ambient;		
		m_antiLight = 0;
	}

	void DirLight::onTick()
	{
		getEntity();
		getEntity()->getCore();

		std::shared_ptr<Shader> _lSh = getEntity()->getCore()->m_lightingSh;
		std::string uniform;
		uniform = "in_dLight[0].m_specIntens";
		float t = getSpec();
		_lSh->setUniform(uniform, getSpec());

		uniform = "in_dLight[0].m_diffuse";
		_lSh->setUniform(uniform, getDif());

		uniform = "in_dLight[0].m_ambient";
		_lSh->setUniform(uniform, getAmb());

		uniform = "in_dLight[0].m_direction";
		_lSh->setUniform(uniform, transform()->getFwd());

		transform()->rotate(glm::vec3(1.0f, 0.0f, 0.0f), 0.5f);
	}


	void PointLight::setValues(glm::vec3 _pos, glm::vec3 _diffuse, float _specular, float _radius, float _quadratic)
	{
		
		m_diffuse = _diffuse;
		m_specIntens = _specular;
		m_radius = _radius;
		m_quadratic = _quadratic;
		m_antiLight = 0;
		
	}

	void SpotLight::setValues(glm::vec3 _pos, glm::vec3 _diffuse, float _specular, float _angle, float _fadeAngle, float _radius, float _quadratic, glm::vec3 _direction)
	{
		m_diffuse = _diffuse;
		m_specIntens = _specular;
		m_angle = _angle;
		m_direction = _direction;
		m_fadeAngle = _fadeAngle;
		m_radius = _radius;
		m_quadratic = _quadratic;
		m_antiLight = 0;
	}

}