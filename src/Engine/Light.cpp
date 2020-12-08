#include <glm/glm.hpp>
#include "Component.h"
#include "Light.h"
#include "Entity.h"
#include "Shader.h"
#include "RenderTexture.h"
#define OUTPUT(vec) std::cout << vec.x << " " << vec.y << " " << vec.z <<std::endl;

namespace Engine
{
	void DirLight::setValues(glm::vec3 _diffuse, float _specular, glm::vec3 _ambient)
	{
		m_diffuse = _diffuse;
		m_specIntens = _specular;
		m_ambient = _ambient;		
		m_antiLight = 0;

		m_SM = std::make_shared<ShadowMap>();
		m_SM->Initialise();
	}
	
	void DirLight::onTick()
	{
		glm::mat4 view(1.0f);
		view = glm::lookAt(transform()->getPosition(), transform()->getPosition() + transform()->getFwd(), transform()->getUp());
		getShadowMap()->setLightSpaceMatrix(glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, -50.0f, 50.0f) * view);

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

		uniform = "in_dLight[0].m_shadowMap";
		_lSh->setUniform(uniform, getShadowMap());

		uniform = "in_dLight[0].m_lightMatrix";
		_lSh->setUniform(uniform, getShadowMap()->getLightSpaceMatrix());
	}


	void PointLight::setValues(glm::vec3 _pos, glm::vec3 _diffuse, float _specular, float _radius, float _quadratic)
	{
		
		m_diffuse = _diffuse;
		m_specIntens = _specular;
		m_radius = _radius;
		m_quadratic = _quadratic;
		m_antiLight = 0;
		
	}

	void SpotLight::setValues(glm::vec3 _color, float _specular, float _angle, float _fadeAngle, float _radius, float _brightness)
	{
		m_diffuse = _color;
		m_specIntens = _specular;
		m_angle = _angle;		
		m_fadeAngle = _fadeAngle;
		m_radius = _radius;
		
		m_brightness = _brightness;
		m_quadratic = 0.027f / m_brightness;
		m_antiLight = 0;

		m_SM = std::make_shared<ShadowMap>();
		m_SM->Initialise();
	}

	

	void SpotLight::onTick()
	{
		glm::mat4 view(1.0f);
		glm::vec3 pos = transform()->getPosition();
		view = glm::lookAt(transform()->getPosition(), transform()->getPosition() + transform()->getFwd(), transform()->getUp());
		getShadowMap()->setLightSpaceMatrix(glm::perspective(glm::radians(getFangle() * 2.0f), 1.0f, 0.1f, getRadius()) * view);		
		m_quadratic = 0.027f / m_brightness;

		std::shared_ptr<Shader> _lSh = getEntity()->getCore()->m_lightingSh;
		std::string uniform;
		uniform = "in_sLight[0].m_specIntens";
		_lSh->setUniform(uniform, getSpec());

		uniform = "in_sLight[0].m_diffuse";
		_lSh->setUniform(uniform, getDif());

		uniform = "in_sLight[0].m_direction";
		_lSh->setUniform(uniform, transform()->getFwd());

		uniform = "in_sLight[0].m_shadowMap";
		_lSh->setUniform(uniform, getShadowMap());

		uniform = "in_sLight[0].m_angle";
		_lSh->setUniform(uniform, glm::cos(glm::radians(getAngle())));

		uniform = "in_sLight[0].m_fadeAngle";
		_lSh->setUniform(uniform, glm::cos(glm::radians(getFangle())));

		uniform = "in_sLight[0].m_pos";
		_lSh->setUniform(uniform, transform()->getPosition());

		uniform = "in_sLight[0].m_radius";
		_lSh->setUniform(uniform, getRadius());

		uniform = "in_sLight[0].m_quadratic";
		_lSh->setUniform(uniform, getQuad());

		uniform = "in_sLight[0].m_antiLight";
		_lSh->setUniform(uniform, 0);

		uniform = "in_sLight[0].m_lightMatrix";
		_lSh->setUniform(uniform, getShadowMap()->getLightSpaceMatrix());

		
	}
}