
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
		std::cout << "Started Shader load" << std::endl; //
		std::string vertShader;
		std::string fragShader;

		std::ifstream file(_vert);
		if (!file.is_open())
		{
			throw std::exception();
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
			std::cout << "Broke mate2" << std::endl;
			throw std::exception();
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
			std::cout << "Broke mate3" << std::endl;
			GLint maxLength = 0;
			glGetShaderiv(vertexShaderId, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(vertexShaderId, maxLength, &maxLength, &errorLog[0]);
			std::cout << &errorLog.at(0) << std::endl;
			throw std::exception();
		}

		GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShaderId, 1, &fragment, NULL);
		glCompileShader(fragmentShaderId);
		success = 0;
		glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
		printShaderInfoLog(fragmentShaderId);
		if (!success)
		{
			std::cout << "Broke mate4" << std::endl; //
			GLint maxLength = 0;
			glGetShaderiv(vertexShaderId, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(vertexShaderId, maxLength, &maxLength, &errorLog[0]);
			std::cout << &errorLog.at(0) << std::endl;
			throw std::exception();//
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
			std::cout << "Broke mate5" << std::endl;
			throw std::exception();
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
			printf("%s\n", infoLog);
			free(infoLog);
		}
	}

	/*
	float v1x = TriPos->getData(l);
	float v1y = TriPos->getData(l + 1);
	float v1z = TriPos->getData(l + 2);

	float v2x = TriPos->getData(l + 3);
	float v2y = TriPos->getData(l + 4);
	float v2z = TriPos->getData(l + 5);

	float v3x = TriPos->getData(l + 6);
	float v3y = TriPos->getData(l + 7);
	float v3z = TriPos->getData(l + 8);


	glm::vec4 vV1 = glm::vec4(v1x, v1y, v1z, 1.0f);
	glm::vec4 vV2 = glm::vec4(v2x, v2y, v2z, 1.0f);
	glm::vec4 vV3 = glm::vec4(v3x, v3y, v3z, 1.0f);

	glm::mat4 model = _objects[i]->getModel();
	vV1 = model * vV1;
	vV2 = model * vV2;
	vV3 = model * vV3;

	glm::vec3 v3V1, v3V2, v3V3;
	v3V1 = vV1;
	v3V2 = vV2;
	v3V3 = vV3;


	float tri[3][3] =
	{
		{ vV1.x, vV1.y, vV1.z },
		{ vV2.x, vV2.y, vV2.z },
		{ vV3.x, vV3.y, vV3.z }
	};

	glm::vec3 boxSpace = m_position;
	glm::vec3 boxHalfLen = (m_scale / 2.0f) * m_size;
	float boxMid[3] = { boxSpace.x, boxSpace.y, boxSpace.z };
	float boxDis[3] = { boxHalfLen.x, boxHalfLen.y, boxHalfLen.z };


	bool collideMesh = utility::triBoxOverlap(boxMid, boxDis, tri);

	if (collideMesh)

	{
		float Vector[3], Vector2[3];

		//3 points of a tri
		float v1[] = { tri[0][0],tri[0][1], tri[0][2] };
		float v2[] = { tri[1][0],tri[1][1], tri[1][2] };
		float v3[] = { tri[2][0],tri[2][1], tri[2][2] };

		Vector[0] = v3[0] - v1[0];
		Vector[1] = v3[1] - v1[1];
		Vector[2] = v3[2] - v1[2];

		Vector2[0] = v2[0] - v1[0];
		Vector2[1] = v2[1] - v1[1];
		Vector2[2] = v2[2] - v1[2];

		float *Normal = utility::CrossProduct(Vector, Vector2);

		glm::vec3 triNorm = glm::normalize(glm::vec3(Normal[0], Normal[1], Normal[2])); */

	void Shader::draw(VertexArray *_vertexArray)
	{
		glUseProgram(m_id);
		glBindVertexArray(_vertexArray->getId());
		//glUniform4f(colorUniformId, 0, 1, 0, 1);

		for (size_t i = 0; i < m_sampler.size(); i++)
		{
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
		if (uniformId == -1) { throw std::exception(); }

		glUseProgram(m_id);
		glUniform4f(uniformId, _value.x, _value.y, _value.z, _value.w);
		glUseProgram(0);
	}

	void Shader::setUniform(std::string _uniform, glm::vec3 _value)
	{
		GLint uniformId = glGetUniformLocation(m_id, _uniform.c_str());
		if (uniformId == -1) { throw std::exception(); }

		glUseProgram(m_id);
		glUniform3f(uniformId, _value.x, _value.y, _value.z);
		glUseProgram(0);
	}

	void Shader::setUniform(std::string _uniform, glm::mat4 _value)
	{
		GLint uniformId = glGetUniformLocation(m_id, _uniform.c_str());
		if (uniformId == -1) { throw std::exception(); }

		glUseProgram(m_id);
		glUniformMatrix4fv(uniformId, 1, GL_FALSE, glm::value_ptr(_value));
		glUseProgram(0);
	}

	void Shader::setUniform(std::string _uniform, float _value)
	{
		GLint uniformId = glGetUniformLocation(m_id, _uniform.c_str());
		if (uniformId == -1)
		{
			throw std::exception();
		}

		glUseProgram(m_id);
		glUniform1f(uniformId, _value);
		glUseProgram(0);
	}

	void Shader::setUniform(std::string _uniform, int _value)
	{
		GLint uniformId = glGetUniformLocation(m_id, _uniform.c_str());
		if (uniformId == -1) { throw std::exception(); }

		glUseProgram(m_id);
		glUniform1i(uniformId, _value);
		glUseProgram(0);
	}

	void Shader::setUniform(std::string _uniform, Texture *_tex)
	{
		GLint uniformId = glGetUniformLocation(m_id, _uniform.c_str());
		if (uniformId == -1) { throw std::exception(); }

		for (size_t i = 0; i < m_sampler.size(); i++)
		{
			if (m_sampler.at(i).m_id == uniformId)
			{
				m_sampler.at(i).m_tex = _tex;
				glUseProgram(m_id);
				glUniform1f(uniformId, i);
				glUseProgram(0);
				return;
			}

		}

		Sampler s;
		s.m_id = uniformId;
		s.m_tex = _tex;
		m_sampler.push_back(s);

		glUseProgram(m_id);
		glUniform1f(uniformId, m_sampler.size() - 1);
		glUseProgram(0);
	}
	
	GLuint Shader::getId()
	{
		return m_id;
	}
}