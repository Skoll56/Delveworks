#include <GL/glew.h>
#include <glm/glm.hpp>
#include <fstream>
#include <iostream>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Exception.h"

namespace Engine
{
	void VertexArray::splitStringWhitespace(std::string& input, std::vector<std::string>& output)
	{
		std::string curr;

		output.clear();

		for (size_t i = 0; i < input.length(); i++)
		{
			if (input.at(i) == ' ' ||
				input.at(i) == '\r' ||
				input.at(i) == '\n' ||
				input.at(i) == '\t')
			{
				if (curr.length() > 0)
				{
					output.push_back(curr);
					curr = "";
				}
			}
			else
			{
				curr += input.at(i);
			}
		}

		if (curr.length() > 0)
		{
			output.push_back(curr);
		}
	}

	void VertexArray::splitString(std::string& input, char splitter, std::vector<std::string>& output)
	{
		std::string curr;

		output.clear();

		for (size_t i = 0; i < input.length(); i++)
		{
			if (input.at(i) == splitter)
			{
				output.push_back(curr);
				curr = "";
			}
			else
			{
				curr += input.at(i);
			}
		}

		if (curr.length() > 0)
		{
			output.push_back(curr);
		}
	}

	VertexArray::VertexArray(std::string path) : m_dirty(false)
	{
		glGenVertexArrays(1, &m_id);

		if (!m_id)
		{
			throw Exception("VertexArray", "Failed to generate Vertex Array Object");
		}

		m_buffers.resize(10);
		std::ifstream file(path.c_str());

		if (!file.is_open())
		{
			throw Exception("VertexArray", "Failed to open OBJ at: " + path);
		}

		std::string line;
		std::vector<std::string> splitLine;
		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> texCoords;
		std::vector<glm::vec3> normals;

		VertexBuffer *positionBuffer = NULL;
		VertexBuffer *texCoordBuffer = NULL;
		VertexBuffer *normalBuffer = NULL;

		while (!file.eof())
		{
			std::getline(file, line);
			if (line.length() < 1) continue;
			splitStringWhitespace(line, splitLine);
			if (splitLine.size() < 1) continue;

			if (splitLine.at(0) == "v")
			{
				if (!positionBuffer) positionBuffer = new VertexBuffer();

				positions.push_back(glm::vec3(
					atof(splitLine.at(1).c_str()),
					atof(splitLine.at(2).c_str()),
					atof(splitLine.at(3).c_str())));
			}
			else if (splitLine.at(0) == "vt")
			{
				if (!texCoordBuffer) texCoordBuffer = new VertexBuffer();

				texCoords.push_back(glm::vec2(
					atof(splitLine.at(1).c_str()),
					1.0f - atof(splitLine.at(2).c_str())));
			}
			else if (splitLine.at(0) == "vn")
			{
				if (!normalBuffer) normalBuffer = new VertexBuffer();

				normals.push_back(glm::vec3(
					atof(splitLine.at(1).c_str()),
					atof(splitLine.at(2).c_str()),
					atof(splitLine.at(3).c_str())));
			}
			else if (splitLine.at(0) == "f")
			{
				std::vector<std::string> subsplit;
				splitString(splitLine.at(1), '/', subsplit);

				positionBuffer->add(positions.at(atoi(subsplit.at(0).c_str()) - 1));
				if (texCoordBuffer) texCoordBuffer->add(texCoords.at(atoi(subsplit.at(1).c_str()) - 1));
				if (normalBuffer) normalBuffer->add(normals.at(atoi(subsplit.at(2).c_str()) - 1));
				splitString(splitLine.at(2), '/', subsplit);
				positionBuffer->add(positions.at(atoi(subsplit.at(0).c_str()) - 1));
				if (texCoordBuffer) texCoordBuffer->add(texCoords.at(atoi(subsplit.at(1).c_str()) - 1));
				if (normalBuffer) normalBuffer->add(normals.at(atoi(subsplit.at(2).c_str()) - 1));
				splitString(splitLine.at(3), '/', subsplit);
				positionBuffer->add(positions.at(atoi(subsplit.at(0).c_str()) - 1));
				if (texCoordBuffer) texCoordBuffer->add(texCoords.at(atoi(subsplit.at(1).c_str()) - 1));
				if (normalBuffer) normalBuffer->add(normals.at(atoi(subsplit.at(2).c_str()) - 1));

				if (splitLine.size() < 5) continue;
				splitString(splitLine.at(3), '/', subsplit);

				positionBuffer->add(positions.at(atoi(subsplit.at(0).c_str()) - 1));
				if (texCoordBuffer) texCoordBuffer->add(texCoords.at(atoi(subsplit.at(1).c_str()) - 1));
				if (normalBuffer) normalBuffer->add(normals.at(atoi(subsplit.at(2).c_str()) - 1));
				splitString(splitLine.at(4), '/', subsplit);
				positionBuffer->add(positions.at(atoi(subsplit.at(0).c_str()) - 1));
				if (texCoordBuffer) texCoordBuffer->add(texCoords.at(atoi(subsplit.at(1).c_str()) - 1));
				if (normalBuffer) normalBuffer->add(normals.at(atoi(subsplit.at(2).c_str()) - 1));
				splitString(splitLine.at(1), '/', subsplit);
				positionBuffer->add(positions.at(atoi(subsplit.at(0).c_str()) - 1));
				if (texCoordBuffer) texCoordBuffer->add(texCoords.at(atoi(subsplit.at(1).c_str()) - 1));
				if (normalBuffer) normalBuffer->add(normals.at(atoi(subsplit.at(2).c_str()) - 1));
			}
		}

		setBuffer("in_Position", positionBuffer);
		if (texCoordBuffer) setBuffer("in_TexCoord", texCoordBuffer);
		if (normalBuffer) setBuffer("in_Normal", normalBuffer);
	}

	void VertexArray::loadObj(std::string path)
	{
		path = "../resources/Models/" + path;
		std::ifstream file(path.c_str());

		if (!file.is_open())
		{
			throw Exception("VertexArray", "Failed to open OBJ at: " + path);
		}

		std::string line;
		std::vector<std::string> splitLine;
		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> texCoords;
		std::vector<glm::vec3> normals;

		VertexBuffer *positionBuffer = NULL;
		VertexBuffer *texCoordBuffer = NULL;
		VertexBuffer *normalBuffer = NULL;

		while (!file.eof())
		{
			std::getline(file, line);
			if (line.length() < 1) continue;
			splitStringWhitespace(line, splitLine);
			if (splitLine.size() < 1) continue;

			if (splitLine.at(0) == "v")
			{
				if (!positionBuffer) positionBuffer = new VertexBuffer();

				positions.push_back(glm::vec3(
					atof(splitLine.at(1).c_str()),
					atof(splitLine.at(2).c_str()),
					atof(splitLine.at(3).c_str())));
			}
			else if (splitLine.at(0) == "vt")
			{
				if (!texCoordBuffer) texCoordBuffer = new VertexBuffer();

				texCoords.push_back(glm::vec2(
					atof(splitLine.at(1).c_str()),
					1.0f - atof(splitLine.at(2).c_str())));
			}
			else if (splitLine.at(0) == "vn")
			{
				if (!normalBuffer) normalBuffer = new VertexBuffer();

				normals.push_back(glm::vec3(
					atof(splitLine.at(1).c_str()),
					atof(splitLine.at(2).c_str()),
					atof(splitLine.at(3).c_str())));
			}
			else if (splitLine.at(0) == "f")
			{
				std::vector<std::string> subsplit;
				splitString(splitLine.at(1), '/', subsplit);

				positionBuffer->add(positions.at(atoi(subsplit.at(0).c_str()) - 1));
				if (texCoordBuffer) texCoordBuffer->add(texCoords.at(atoi(subsplit.at(1).c_str()) - 1));
				if (normalBuffer) normalBuffer->add(normals.at(atoi(subsplit.at(2).c_str()) - 1));
				splitString(splitLine.at(2), '/', subsplit);
				positionBuffer->add(positions.at(atoi(subsplit.at(0).c_str()) - 1));
				if (texCoordBuffer) texCoordBuffer->add(texCoords.at(atoi(subsplit.at(1).c_str()) - 1));
				if (normalBuffer) normalBuffer->add(normals.at(atoi(subsplit.at(2).c_str()) - 1));
				splitString(splitLine.at(3), '/', subsplit);
				positionBuffer->add(positions.at(atoi(subsplit.at(0).c_str()) - 1));
				if (texCoordBuffer) texCoordBuffer->add(texCoords.at(atoi(subsplit.at(1).c_str()) - 1));
				if (normalBuffer) normalBuffer->add(normals.at(atoi(subsplit.at(2).c_str()) - 1));

				if (splitLine.size() < 5) continue;
				splitString(splitLine.at(3), '/', subsplit);

				positionBuffer->add(positions.at(atoi(subsplit.at(0).c_str()) - 1));
				if (texCoordBuffer) texCoordBuffer->add(texCoords.at(atoi(subsplit.at(1).c_str()) - 1));
				if (normalBuffer) normalBuffer->add(normals.at(atoi(subsplit.at(2).c_str()) - 1));
				splitString(splitLine.at(4), '/', subsplit);
				positionBuffer->add(positions.at(atoi(subsplit.at(0).c_str()) - 1));
				if (texCoordBuffer) texCoordBuffer->add(texCoords.at(atoi(subsplit.at(1).c_str()) - 1));
				if (normalBuffer) normalBuffer->add(normals.at(atoi(subsplit.at(2).c_str()) - 1));
				splitString(splitLine.at(1), '/', subsplit);
				positionBuffer->add(positions.at(atoi(subsplit.at(0).c_str()) - 1));
				if (texCoordBuffer) texCoordBuffer->add(texCoords.at(atoi(subsplit.at(1).c_str()) - 1));
				if (normalBuffer) normalBuffer->add(normals.at(atoi(subsplit.at(2).c_str()) - 1));
			}
		}

		setBuffer("in_Position", positionBuffer);
		if (texCoordBuffer) setBuffer("in_TexCoord", texCoordBuffer);
		if (normalBuffer) setBuffer("in_Normal", normalBuffer);
	}

	VertexArray::VertexArray() : m_dirty(false)
	{
		glGenVertexArrays(1, &m_id);

		if (!m_id)
		{
			throw Exception("VertexArray", "Failed to generate Vertex Array");
		}

		m_buffers.resize(10);
	}

	void VertexArray::setBuffer(std::string attribute, VertexBuffer *buffer)
	{
		if (attribute == "in_Position")
		{
			m_buffers.at(0) = buffer;
		}
		else if (attribute == "in_Color")
		{
			m_buffers.at(3) = buffer;
		}
		else if (attribute == "in_TexCoord")
		{
			m_buffers.at(2) = buffer;
		}
		else if (attribute == "in_Normal")
		{
			m_buffers.at(1) = buffer;
		}
		else
		{
			throw Exception("VertexArray", "Failed to set attribute in Vertex Array: " + attribute);
		}

		m_dirty = true;
	}

	int VertexArray::getVertexCount()
	{
		if (!m_buffers.at(0))
		{
			throw Exception("VertexArray", "Failed to read Vertex Count (Buffer empty)");
		}

		return m_buffers.at(0)->getDataSize() / m_buffers.at(0)->getComponents();
	}

	GLuint VertexArray::getId()
	{
		if (m_dirty)
		{
			glBindVertexArray(m_id);

			for (size_t i = 0; i < m_buffers.size(); i++)
			{
				if (m_buffers.at(i))
				{
					glBindBuffer(GL_ARRAY_BUFFER, m_buffers.at(i)->getId());

					glVertexAttribPointer(i, m_buffers.at(i)->getComponents(), GL_FLOAT, GL_FALSE,
						m_buffers.at(i)->getComponents() * sizeof(GLfloat), (void *)0);

					glEnableVertexAttribArray(i);
				}
				else
				{
					glDisableVertexAttribArray(i);
				}
			}

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			m_dirty = false;
		}

		return m_id;
	}
}
