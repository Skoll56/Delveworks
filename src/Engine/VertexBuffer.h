#ifndef _VertexBuffer_H_
#define _VertexBuffer_H_
#include <vector>
#include <GL/glew.h>

namespace Engine
{
	class VertexBuffer

	{
	private:
		GLuint m_id;
		int m_components;
		std::vector <GLfloat> m_data;
		bool m_dirty;


	public:
		VertexBuffer();
		float getData(int i) { return m_data[i]; }
		void add(glm::vec2 _value);
		void add(glm::vec3 _value);
		void add(glm::vec4 _value);
		int getComponents();
		int getDataSize();
		GLuint getId();

	};
}

#endif