#ifndef _VertexArray_H_
#define _VertexArray_H_
#include <vector>
#include <string>
#include <GL/glew.h>
#include "Resource.h"

namespace Engine
{
	class VertexBuffer;

	class VertexArray : public Resource
	{
		GLuint m_id;
		bool m_dirty;
		std::vector<VertexBuffer *> m_buffers;

		void splitStringWhitespace(std::string& _input, std::vector<std::string>& _output);
		void splitString(std::string& _input, char _splitter, std::vector<std::string>& _output);

	public:

		VertexArray();
		VertexArray(std::string _path);
		VertexBuffer* getTriPos() { return m_buffers.at(0); }
		VertexBuffer* getTriTex() { return m_buffers.at(2); }
		VertexBuffer* getTriNorm() { return m_buffers.at(1); } //Changed from 2, TODO: Check mesh collision normals
		void load(std::string _path);
		void setBuffer(std::string _attribute, VertexBuffer *_buffer);
		int getVertexCount();
		GLuint getId();

	};
}

#endif