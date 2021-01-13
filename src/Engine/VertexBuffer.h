#ifndef _VertexBuffer_H_
#define _VertexBuffer_H_
#include <vector>
#include <GL/glew.h>

namespace Engine
{
	class VertexBuffer

	{
		private:
		/** \brief The ID of the Vertexbuffer*/
		GLuint m_id;

		/** \brief How many components (not to be confused with the class, Component) the buffer has*/
		int m_components;

		/** \brief The data in the buffer*/
		std::vector <GLfloat> m_data;
		bool m_dirty;


		public:
		VertexBuffer();
		/** \brief Gets the data from a position in the buffer*/
		float getData(int i) { return m_data[i]; }

		/** \brief Adds data to the buffer*/
		void add(glm::vec2 _value);

		/** \brief Adds data to the buffer*/
		void add(glm::vec3 _value);

		/** \brief Adds data to the buffer*/
		void add(glm::vec4 _value);

		/** \brief Returns the number of components*/
		int getComponents();

		/** \brief Returns the length of the data in the buffer*/
		int getDataSize();

		/** \brief Returns the ID of the buffer*/
		GLuint getId();
	};
}

#endif