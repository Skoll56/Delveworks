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
		/** \brief The Vertex Array ID*/
		GLuint m_id;		
		bool m_dirty;

		/** \brief A vector of Buffers that the Vertex Array holds (Such as texture co-ordinates, normal buffer and vertex position buffer)*/
		std::vector<VertexBuffer*> m_buffers;

		/** \brief Used for loading in OBJ files*/
		void splitStringWhitespace(std::string& _input, std::vector<std::string>& _output);

		/** \brief Used for loading in OBJ files*/
		void splitString(std::string& _input, char _splitter, std::vector<std::string>& _output);

		public:
		VertexArray();
		VertexArray(std::string _path);

		/** \brief Returns the array of Triangle Vertex positions*/
		VertexBuffer* getTriPos() { return m_buffers.at(0); }

		/** \brief Returns the array of texture coordinates*/
		VertexBuffer* getTriTex() { return m_buffers.at(2); }

		/** \brief Returns the array of surface normals*/
		VertexBuffer* getTriNorm() { return m_buffers.at(1); } //Changed from 2, TODO: Check mesh collision normals

		/** \brief Loads an OBJ from file*/
		void load(std::string _path);

		/** \brief Used to add a buffer to the object*/
		void setBuffer(std::string _attribute, VertexBuffer *_buffer);

		/** \brief Returns how many vertexes an object has*/
		int getVertexCount();

		/** \brief Returns the ID of the Mesh*/
		GLuint getId();

	};
}

#endif