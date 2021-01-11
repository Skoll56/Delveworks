#include <glm/glm.hpp>
#include "Component.h"
#include "Light.h"
#include "Entity.h"
#include "Shader.h"
#include "RenderTexture.h"


namespace Engine
{
	/* !This has been modified as part of the GRAPHICS UNIT to support shadows! */
	/* !The ability to cast lights (but not shadows) was already a feature that has been submitted before! */

	void DirLight::onInitialise(glm::vec3 _diffuse, float _specular, glm::vec3 _ambient)
	{
		m_diffuse = _diffuse;
		m_specIntens = _specular;
		m_ambient = _ambient;		
		m_antiLight = 0;		

		m_SM = std::make_shared<ShadowMap>();
		m_SM->Initialise(2048.0f * 4.0f, 2048.0f * 4.0f);
	}

	void DirLight::update(int _i) //NOTE
	{
		std::shared_ptr<ShadowMap> sm = getShadowMap();

		glm::mat4 view(1.0f);
		glm::vec3 camPos = getEntity()->getCore()->getDefaultCamera()->transform()->getPosition();
		view = glm::lookAt(camPos + glm::vec3(0.0f, 100.0f, 0.0f), camPos + glm::vec3(0.0f, 100.0f, 0.0f) + transform()->getFwd(), transform()->getUp());
		sm->setLightSpaceMatrix(glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, 0.0f, 200.0f) * view);

		std::shared_ptr<Shader> _lSh = getEntity()->getCore()->m_lightingSh;
		std::string uniform;
		std::string itr = std::to_string(_i);

		

		uniform = "in_dLight[" + itr + "].m_specIntens";
		float t = getSpec();
		_lSh->setUniform(uniform, getSpec());

		uniform = "in_dLight[" + itr + "].m_diffuse";
		_lSh->setUniform(uniform, getDif());

		uniform = "in_dLight[" + itr + "].m_ambient";
		_lSh->setUniform(uniform, getAmb());

		uniform = "in_dLight[" + itr + "].m_direction";
		_lSh->setUniform(uniform, transform()->getFwd());

		uniform = "in_dirShadowMap[" + itr + "]";
		_lSh->setUniform(uniform, sm);

		uniform = "in_dLight[" + itr + "].m_textureSize";
		_lSh->setUniform(uniform, glm::vec2(sm->resolutionX, sm->resolutionY));

		uniform = "in_dLight[" + itr + "].m_lightMatrix";
		_lSh->setUniform(uniform, sm->getLightSpaceMatrix());
	}

	void PointLight::onInitialise(glm::vec3 _diffuse, float _specular, float _radius, float _brightness)
	{		
		m_diffuse = _diffuse;
		m_specIntens = _specular;
		m_radius = _radius;
		m_quadratic = 0.027f / m_brightness / _brightness;
		m_brightness = _brightness;
		m_antiLight = 0;		

		m_SC = std::make_shared<ShadowCube>();
		m_SC->Initialise();
	}

	void SpotLight::onInitialise(glm::vec3 _color, float _specular, float _angle, float _fadeAngle, float _radius, float _brightness)
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
		m_SM->Initialise(1024.0f, 1024.0f);
	}

	void SpotLight::update(int _i)
	{
		glm::mat4 view(1.0f);
		std::shared_ptr<ShadowMap> sm = getShadowMap();

		view = glm::lookAt(transform()->getPosition(), transform()->getPosition() + transform()->getFwd(), transform()->getUp());
		sm->setLightSpaceMatrix(glm::perspective(glm::radians(getFangle() * 2.0f), 1.0f, 0.1f, getRadius()) * view);
		m_quadratic = 0.027f / m_brightness;

		std::shared_ptr<Shader> _lSh = getCore()->m_lightingSh;
		std::string uniform;	
		std::string itr = std::to_string(_i);
		uniform = "in_sLight[" + itr + "].m_specIntens";
		_lSh->setUniform(uniform, getSpec());

		uniform = "in_sLight[" + itr + "].m_diffuse";
		_lSh->setUniform(uniform, getDif());

		uniform = "in_sLight[" + itr + "].m_direction";
		_lSh->setUniform(uniform, transform()->getFwd());

		uniform = "in_spotShadowMap[" + itr + "]"; 
		_lSh->setUniform(uniform, sm);

		uniform = "in_sLight[" + itr + "].m_angle";
		_lSh->setUniform(uniform, glm::cos(glm::radians(getAngle())));

		uniform = "in_sLight[" + itr + "].m_fadeAngle";
		_lSh->setUniform(uniform, glm::cos(glm::radians(getFangle())));

		uniform = "in_sLight[" + itr + "].m_pos";
		_lSh->setUniform(uniform, transform()->getPosition());

		uniform = "in_sLight[" + itr + "].m_radius";
		_lSh->setUniform(uniform, getRadius());

		uniform = "in_sLight[" + itr + "].m_quadratic";
		_lSh->setUniform(uniform, getQuad());

		uniform = "in_sLight[" + itr + "].m_textureSize";
		_lSh->setUniform(uniform, glm::vec2(sm->resolutionX, sm->resolutionY));

		uniform = "in_sLight[" + itr + "].m_lightMatrix";
		_lSh->setUniform(uniform, sm->getLightSpaceMatrix());
	}

	void PointLight::update(int _i)
	{
		glm::mat4 view(1.0f);
		std::shared_ptr<ShadowCube> SC = getShadowCube();
		
		view = glm::lookAt(transform()->getPosition(), transform()->getPosition() + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		glm::mat4 zero = (glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, getRadius()) * view);
		SC->setMatrix(0, zero);

		view = glm::lookAt(transform()->getPosition(), transform()->getPosition() + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		glm::mat4 one = (glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, getRadius()) * view);
		SC->setMatrix(1, one);

		view = glm::lookAt(transform()->getPosition(), transform()->getPosition() + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 two = (glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, getRadius()) * view);
		SC->setMatrix(2, two);

		view = glm::lookAt(transform()->getPosition(), transform()->getPosition() + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
		glm::mat4 three = (glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, getRadius()) * view);
		SC->setMatrix(3, three);

		view = glm::lookAt(transform()->getPosition(), transform()->getPosition() + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		glm::mat4 four = (glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, getRadius()) * view);
		SC->setMatrix(4, four);

		view = glm::lookAt(transform()->getPosition(), transform()->getPosition() + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		glm::mat4 five = (glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, getRadius()) * view);
		SC->setMatrix(5, five);

		m_quadratic = 0.027f / m_brightness;
		std::shared_ptr<Shader> _lSh = getEntity()->getCore()->m_lightingSh;
		std::string uniform;
		std::string itr = std::to_string(_i);
		uniform = "in_pLight[" + itr + "].m_specIntens";
		_lSh->setUniform(uniform, getSpec());

		uniform = "in_pLight[" + itr + "].m_diffuse";
		_lSh->setUniform(uniform, getDif());		
		
		uniform = "in_pointShadowMap[" + itr + "]"; //
		_lSh->setUniform(uniform, SC);

		uniform = "in_pLight[" + itr + "].m_pos";
		_lSh->setUniform(uniform, transform()->getPosition());

		uniform = "in_pLight[" + itr + "].m_radius";//
		_lSh->setUniform(uniform, getRadius());

		//uniform = "in_pLight[" + itr + "].m_textureSize";
		//_lSh->setUniform(uniform, glm::vec2(SC->resolutionX, SC->resolutionY));

		uniform = "in_pLight[" + itr + "].m_quadratic";
		_lSh->setUniform(uniform, getQuad());		
	}
}