#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <glm/glm.hpp>
#include "Component.h"

namespace Engine
{
	class Light : public Component
	{
	public:		
		glm::vec3 getDif() { return m_diffuse; }
		float getSpec() { return m_specIntens; }
		bool isAnti() { return m_antiLight; }
		void setAnti(bool _val) { m_antiLight = _val; }
		std::string getTag() { return m_tag; }
		void setTag(std::string _tag) { m_tag = _tag; }

	protected:		
		glm::vec3 m_diffuse;
		float m_specIntens;
		int m_antiLight;
		std::string m_tag;
	};


	class PointLight : public Light
	{
	public:
		PointLight() {};
		void setValues(glm::vec3 _pos, glm::vec3 _diffuse, float _specular, float _radius, float _quadratic);
		
		float getRadius() { return m_radius; }
		float getQuad() { return m_quadratic; }

	protected:

		float m_radius;
		float m_quadratic;
	};

	class DirLight : public Light
	{
	public:
		void setValues(glm::vec3 _diffuse, float _specular, glm::vec3 _ambient, glm::vec3 _direction);

		glm::vec3 getDif() { return m_diffuse; }
		float getSpec() { return m_specIntens; }

		glm::vec3 m_diffuse;
		float m_specIntens;
		glm::vec3 getDirection() { return m_direction; }
		glm::vec3 getAmb() { return m_ambient; }
		void onTick();

	private:
		glm::vec3 m_direction;
		glm::vec3 m_ambient;
	};


	class SpotLight : public PointLight
	{
	public:
		void setValues(glm::vec3 _pos, glm::vec3 _diffuse, float _specular, float _angle, float _fadeAngle, float _radius, float _quadratic, glm::vec3 _direction);
		float getAngle() { return m_angle; }
		float getFangle() { return m_fadeAngle; }
		glm::vec3 getDirection() { return m_direction; }
		void setDirection(glm::vec3 _dir) { m_direction = _dir; }


	private:
		float m_angle;
		float m_fadeAngle;
		glm::vec3 m_direction;
	};
}
#endif