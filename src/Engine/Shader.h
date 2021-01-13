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

	/** \brief A data structure that contains a Texture and is used to help wrap it*/
	struct Sampler
	{
		/** \brief Save the uniform location of the Texture*/
		GLint m_uniform_location;

		/** \brief The Texture*/
		std::shared_ptr<Texture> m_tex;
	};

	/** \brief A Shader is used to make Graphics*/
	/** Unless you have edited the shaders, you shouldn't need to use these functions  */
	class Shader : public Resource
	{
		private:
		/** \brief The ID of the shader program*/
		GLuint m_id;
		/** \brief The list of Samplers to draw*/
		std::vector<Sampler> m_sampler;
		
		/** \brief A function for debugging shaders at compilation*/
		void printShaderInfoLog(GLuint obj); //Reference Karsten

		public:
		Shader();
		Shader(std::string _name);

		/** \brief Loads a shader from file*/
		void load(std::string _name);

		/** \brief draws an object's mesh*/
		void draw(std::shared_ptr<VertexArray> vertexArray);

		
		/** \brief Sets a Uniform of type Vec4*/
		void setUniform(std::string _uniform, glm::vec4 _value);

		/** \brief Sets a Uniform of type float*/
		void setUniform(std::string _uniform, float _value);

		/** \brief Sets a Uniform of type int*/
		void setUniform(std::string _uniform, int _value);

		/** \brief Sets a Uniform of type Texture*/
		void setUniform(std::string _uniform, std::shared_ptr<Texture> _tex); /* !This has been modified as part of the GRAPHICS UNIT! */

		/** \brief Sets a Uniform of type ShadowCube*/
		void setUniform(std::string _uniform, std::shared_ptr<ShadowCube> _sc); /* !This has been modified as part of the GRAPHICS UNIT! */

		/** \brief Sets a Uniform of type Mat4*/
		void setUniform(std::string _uniform, glm::mat4 _value);

		/** \brief Sets a Uniform of type Vec3*/
		void setUniform(std::string _uniform, glm::vec3 _value);

		/** \brief Sets a Uniform of type Vec2*/
		void setUniform(std::string _uniform, glm::vec2 _value);

		/** \brief Returns the ID of the shader program*/
		GLuint getId();
	};
}

#endif