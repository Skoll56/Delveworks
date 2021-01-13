#ifndef _Texture_H_
#define _Texture_H_

#include <GL/glew.h>
#include "Resource.h"

namespace Engine
{
	/** \brief  A texture is usually a loaded image*/
	/** Textures can be applied to ImageUI as well as MeshRenderer objects */
	class Texture : public Resource
	{
		public:
		Texture();
		~Texture() {};

		/** \brief Returns the ID of the texture*/
		GLuint get() { return m_textureId; }

		/** \brief Loads a texture*/
		void load(std::string _image);

		/** \brief Returns the resolution of the Texture*/
		glm::vec2 getResolution() { return glm::vec2(m_resX, m_resY); }
		
		protected:
		/** \brief Width of the texture*/
		int m_resX;

		/** \brief Height of the texture*/
		int	m_resY; 

		/** \brief How many channels it has*/
		int	m_channels;

		/** \brief The texture ID*/
		GLuint m_textureId;		
	};
}

#endif