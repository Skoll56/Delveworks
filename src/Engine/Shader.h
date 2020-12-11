#ifndef _SHADER_H_
#define _SHADER_H_
#include <vector>
#include <GL/glew.h>
#include "Texture.h"

namespace Engine
{
	class VertexArray;
	class Texture;
	class SpotLight;
	class PointLight;
	class DirLight;
	class ShadowCube;


	struct Sampler
	{
		GLint m_uniform_location;
		std::shared_ptr<Texture> m_tex;
	};

	class Shader : public Resource
	{
	private:
		GLuint m_id;
		std::vector<Sampler> m_sampler;
		
	public:
		Shader();
		Shader(std::string _name);
		void load(std::string _name);
		void overrideDraw(VertexArray * _vertexArray);
		void draw(VertexArray *vertexArray);
		void printShaderInfoLog(GLuint obj); //Reference Karsten
		void setUniform(std::string _uniform, glm::vec4 _value);
		void setUniform(std::string _uniform, float _value);
		void setUniform(std::string _uniform, int _value);
		void setUniform(std::string _uniform, std::shared_ptr<Texture> _tex);
		void setUniform(std::string _uniform, std::shared_ptr<ShadowCube> _sc);
		void setUniform(std::string _uniform, glm::mat4 _value);
		void setUniform(std::string _uniform, glm::vec3 _value);
		GLuint getId();
	};
}

#endif