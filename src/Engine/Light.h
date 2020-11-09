#include <glm/glm.hpp>

namespace Engine
{
	class Light
	{
	public:
		glm::vec3 getPos() { return m_pos; }
		void setPos(glm::vec3 _pos) { m_pos = _pos; }
		glm::vec3 getDif() { return m_diffuse; }
		float getSpec() { return m_specIntens; }
		bool isAnti() { return m_antiLight; }
		void setAnti(bool _val) { m_antiLight = _val; }
		std::string getTag() { return m_tag; }
		void setTag(std::string _tag) { m_tag = _tag; }

	protected:
		glm::vec3 m_pos;
		glm::vec3 m_diffuse;
		float m_specIntens;
		int m_antiLight;
		std::string m_tag;
	};


	class PointLight : public Light
	{
	public:
		PointLight() {};
		PointLight(glm::vec3 _pos, glm::vec3 _diffuse, float _specular, float _radius, float _quadratic)
		{
			m_pos = _pos;
			m_diffuse = _diffuse;
			m_specIntens = _specular;
			m_radius = _radius;
			m_quadratic = _quadratic;
			m_antiLight = 0;
		}
		float getRadius() { return m_radius; }
		float getQuad() { return m_quadratic; }

	protected:

		float m_radius;
		float m_quadratic;
	};

	class DirLight : public Light
	{
	public:
		DirLight(glm::vec3 _diffuse, float _specular, glm::vec3 _ambient, glm::vec3 _direction)
		{
			m_diffuse = _diffuse;
			m_specIntens = _specular;
			m_ambient = _ambient;
			m_direction = _direction;
			m_antiLight = 0;
		}

		glm::vec3 getDif() { return m_diffuse; }
		float getSpec() { return m_specIntens; }

		glm::vec3 m_diffuse;
		float m_specIntens;
		glm::vec3 getDirection() { return m_direction; }
		glm::vec3 getAmb() { return m_ambient; }

	private:
		glm::vec3 m_direction;
		glm::vec3 m_ambient;
	};


	class SpotLight : public PointLight
	{
	public:
		SpotLight(glm::vec3 _pos, glm::vec3 _diffuse, float _specular, float _angle, float _fadeAngle, float _radius, float _quadratic, glm::vec3 _direction)
		{
			m_pos = _pos;
			m_diffuse = _diffuse;
			m_specIntens = _specular;
			m_angle = _angle;
			m_direction = _direction;
			m_fadeAngle = _fadeAngle;
			m_radius = _radius;
			m_quadratic = _quadratic;
			m_antiLight = 0;
		}
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