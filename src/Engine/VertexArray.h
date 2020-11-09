#ifndef _VertexArray_H_
#define _VertexArray_H_
#include <vector>
#include <string>
#include <GL/glew.h>

namespace Engine
{
	class VertexBuffer;

	class VertexArray
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
		VertexBuffer* getTriNorm() { return m_buffers.at(2); }
		void loadObj(std::string _path);
		void setBuffer(std::string _attribute, VertexBuffer *_buffer);
		int getVertexCount();
		GLuint getId();

	};
}

#endif