#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include "VertexArray.h"
#include "Shader.h"
#include "Light.h"
#include "RenderTexture.h"
#include "Exception.h"
#include "Console.h"


namespace Engine
{
	Shader::Shader()
	{
	}
	Shader::Shader(std::string _name)
	{		
		load(_name);
	}

	void Shader::load(std::string  _name)
	{
		std::string _vert, _frag;
		_vert = "../resources/shaders/" + _name + ".vert";
		_frag = "../resources/shaders/" + _name + ".frag";
		Console::output("Started Shader load: " + _name ); 
		std::string vertShader;
		std::string fragShader;

		std::ifstream file(_vert);
		if (!file.is_open())
		{
			throw Exception("Failed to open vertex shader file");
		}
		else
		{
			while (!file.eof())
			{
				std::string line;
				std::getline(file, line);
				vertShader += line + "\n";

			}
		}
		file.close();
		file.open(_frag);
		if (!file.is_open())
		{
			throw Exception("Failed to open fragment shader file");
		}
		else
		{
			while (!file.eof())
			{
				std::string line;
				std::getline(file, line);
				fragShader += line + "\n";

			}
		}
		file.close();

		const char *vertex = vertShader.c_str();
		const char *fragment = fragShader.c_str();


		GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShaderId, 1, &vertex, NULL);
		glCompileShader(vertexShaderId);
		GLint success = 0;
		glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
		if (!success)
		{			
			GLint maxLength = 0;
			glGetShaderiv(vertexShaderId, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(vertexShaderId, maxLength, &maxLength, &errorLog[0]);			
			std::string error = &errorLog.at(0);
			throw Exception("Failed to compile vertex shader: " + error);
		}

		GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShaderId, 1, &fragment, NULL);
		glCompileShader(fragmentShaderId);
		success = 0;
		glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
		printShaderInfoLog(fragmentShaderId);
		if (!success)
		{			
			GLint maxLength = 0;
			glGetShaderiv(vertexShaderId, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(vertexShaderId, maxLength, &maxLength, &errorLog[0]);
			std::string error = &errorLog.at(0);
			throw Exception("Failed to compile fragment shader: " + error);
		}
		m_id = glCreateProgram();
		glAttachShader(m_id, vertexShaderId);
		glAttachShader(m_id, fragmentShaderId);
		glBindAttribLocation(m_id, 0, "in_Position");
		//glBindAttribLocation(m_id, 1, "in_Color");
		glBindAttribLocation(m_id, 2, "in_TexCoord");
		//glBindAttribLocation(m_id, 3, "in_FragPos");
		//glBindAttribLocation(m_id, 1, "in_Normal");


		glLinkProgram(m_id);
		success = 0;
		glGetProgramiv(m_id, GL_LINK_STATUS, &success);
		if (!success)
		{			
			GLint maxLength = 0;
			glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_id, maxLength, &maxLength, &infoLog[0]);

			// The program is useless now. So delete it.
			glDeleteProgram(m_id);

			// Provide the infolog in whatever manner you deem best.
			std::string error = "";
			for (int i = 0; i < infoLog.size(); i++)
			{
				error += infoLog[i];
			}
			// Exit with failure.			
			throw Exception("Failed to link shader program: " + error);
		}

		//glDetachShader(m_id, vertexShaderId);
		glDeleteShader(vertexShaderId);
		//glDetachShader(m_id, fragmentShaderId);
		glDeleteShader(fragmentShaderId);
	}

	void Shader::printShaderInfoLog(GLuint obj)
	{
		int infologLength = 0;
		int charsWritten = 0;
		char *infoLog;

		glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

		if (infologLength > 0)
		{
			infoLog = (char *)malloc(infologLength);
			glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
			Console::output(Console::Warning, "Shader", std::string(infoLog));
			free(infoLog);
		}
	}

	void Shader::draw(std::shared_ptr<VertexArray> _vertexArray)
	{
		glUseProgram(m_id);
		glBindVertexArray(_vertexArray->getId());
		//glUniform4f(colorUniformId, 0, 1, 0, 1);

		for (size_t i = 0; i < m_sampler.size(); i++)
		{
			glUniform1i(m_sampler.at(i).m_uniform_location, i);
			glActiveTexture(GL_TEXTURE0 + i);
			if (m_sampler.at(i).m_tex)
			{
				glBindTexture(GL_TEXTURE_2D, m_sampler.at(i).m_tex->get());
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, 0);
			}

		}
		glDrawArrays(GL_TRIANGLES, 0, _vertexArray->getVertexCount());

		for (size_t i = 0; i < m_sampler.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		glBindVertexArray(0);
		glUseProgram(0);

	}

	void Shader::setUniform(std::string _uniform, glm::vec4 _value)
	{
		GLint uniformId = glGetUniformLocation(m_id, _uniform.c_str());
		if (uniformId == -1) 
		{ 
			throw Exception("Failed to set uniform: (" + _uniform + ")"); 
		}

		glUseProgram(m_id);
		glUniform4f(uniformId, _value.x, _value.y, _value.z, _value.w);
		glUseProgram(0);
	}

	void Shader::setUniform(std::string _uniform, glm::vec3 _value)
	{
		GLint uniformId = glGetUniformLocation(m_id, _uniform.c_str());
		if (uniformId == -1)
		{
			throw Exception("Failed to set uniform: (" + _uniform + ")");
		}

		glUseProgram(m_id);
		glUniform3f(uniformId, _value.x, _value.y, _value.z);
		glUseProgram(0);
	}

	void Shader::setUniform(std::string _uniform, glm::vec2 _value)
	{
		GLint uniformId = glGetUniformLocation(m_id, _uniform.c_str());
		if (uniformId == -1)
		{
			throw Exception("Failed to set uniform: (" + _uniform + ")");
		}

		glUseProgram(m_id);
		glUniform2f(uniformId, _value.x, _value.y);
		glUseProgram(0);
	}

	void Shader::setUniform(std::string _uniform, glm::mat4 _value)
	{
		GLint uniformId = glGetUniformLocation(m_id, _uniform.c_str());
		if (uniformId == -1)
		{
			throw Exception("Failed to set uniform: (" + _uniform + ")");
		}

		glUseProgram(m_id);
		glUniformMatrix4fv(uniformId, 1, GL_FALSE, glm::value_ptr(_value));
		glUseProgram(0);
	}

	void Shader::setUniform(std::string _uniform, float _value)
	{
		GLint uniformId = glGetUniformLocation(m_id, _uniform.c_str());
		if (uniformId == -1)
		{
			throw Exception("Failed to set uniform: (" + _uniform + ")");
		}

		glUseProgram(m_id);
		glUniform1f(uniformId, _value);
		glUseProgram(0);
	}

	void Shader::setUniform(std::string _uniform, int _value)
	{
		GLint uniformId = glGetUniformLocation(m_id, _uniform.c_str());
		if (uniformId == -1)
		{
			throw Exception("Failed to set uniform: (" + _uniform + ")");
		}
		glUseProgram(m_id);
		glUniform1i(uniformId, _value);
		glUseProgram(0);
	}

	/* !This has been modified as part of the GRAPHICS UNIT! */
	void Shader::setUniform(std::string _uniform, std::shared_ptr<Texture> _tex)
	{
		GLint uniformId = glGetUniformLocation(m_id, _uniform.c_str());
		if (uniformId == -1)
		{
			if (_tex->m_path == "")
			{
				throw Exception("Failed to set uniform: (" + _uniform + ")");
			}
			else
			{
				throw Exception("Failed to set uniform: (" + _uniform + ") to " + _tex->m_path);
			}			
		}

		for (size_t i = 0; i < m_sampler.size(); i++)
		{
			if (m_sampler.at(i).m_uniform_location == uniformId)
			{
				m_sampler.at(i).m_tex = _tex;
				glUseProgram(m_id);
				glUniform1i(uniformId, i);
				glUseProgram(0);
				return;
			}
		}

		Sampler s;
		s.m_uniform_location = uniformId;
		s.m_tex = _tex;
		m_sampler.push_back(s);

		glUseProgram(m_id);
		glUniform1i(uniformId, m_sampler.size() - 1);
		glActiveTexture(GL_TEXTURE0 + m_sampler.size() - 1);
		glBindTexture(GL_TEXTURE_2D, _tex->get());
		glActiveTexture(GL_TEXTURE0);
		glUseProgram(0);
	}

	/* !This has been CREATED as part of the GRAPHICS UNIT! */
	void Shader::setUniform(std::string _uniform, std::shared_ptr<ShadowCube> _sc)
	{
		GLint uniformId = glGetUniformLocation(m_id, _uniform.c_str());
		if (uniformId == -1)
		{
			throw Exception("Failed to set uniform: (" + _uniform + ")");
		}

		for (size_t i = 0; i < m_sampler.size(); i++)
		{
			if (m_sampler.at(i).m_uniform_location == uniformId)
			{
				m_sampler.at(i).m_tex = _sc;
				glUseProgram(m_id);
				glUniform1i(uniformId, i);
				glUseProgram(0);
				return;
			}
		}

		Sampler s;
		s.m_uniform_location = uniformId;
		s.m_tex = _sc;
		m_sampler.push_back(s);

		glUseProgram(m_id);
		glUniform1i(uniformId, m_sampler.size() - 1);
		glActiveTexture(GL_TEXTURE0 + m_sampler.size() - 1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, _sc->get());
		glActiveTexture(GL_TEXTURE0);
		glUseProgram(0);
	}
	
	GLuint Shader::getId()
	{
		return m_id;
	}
}