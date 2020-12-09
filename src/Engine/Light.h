#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <glm/glm.hpp>
#include "Component.h"

namespace Engine
{
	class ShadowMap;
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
		void setValues(glm::vec3 _diffuse, float _specular, float _radius, float _brightness);
		std::vector<std::shared_ptr<ShadowMap>> getShadowCube() { return m_SC; }
		float getRadius() { return m_radius; }
		float getQuad() { return m_quadratic; }
		void update(int _i);

	protected:
		std::vector<std::shared_ptr<ShadowMap>> m_SC;
		float m_radius;
		float m_brightness;
		float m_quadratic;
		
	};

	class DirLight : public Light
	{
	public:
		void setValues(glm::vec3 _diffuse, float _specular, glm::vec3 _ambient);

		glm::vec3 getDif() { return m_diffuse; }
		float getSpec() { return m_specIntens; }
		std::shared_ptr<ShadowMap> getShadowMap() { return m_SM; }
		glm::vec3 m_diffuse;
		float m_specIntens;		
		glm::vec3 getAmb() { return m_ambient; }
		void update(int _i);
	private:
		
		glm::vec3 m_ambient;
		std::shared_ptr<ShadowMap> m_SM;
	};


	class SpotLight : public PointLight
	{
	public:
		void setValues(glm::vec3 _color, float _specular, float _angle, float _fadeAngle, float _radius, float _brightness);
		void update(int _i);
		float getAngle() { return m_angle; }
		float getFangle() { return m_fadeAngle; }
		std::shared_ptr<ShadowMap> getShadowMap() { return m_SM; }
	private:
		float m_angle;
		std::shared_ptr<ShadowMap> m_SM;
		float m_fadeAngle;		
	};
}
#endif