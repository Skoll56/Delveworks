#ifndef _Texture_H_
#define _Texture_H_

#include <GL/glew.h>
#include "Resource.h"

namespace Engine
{
	class Texture : public Resource
	{
	public:
		Texture();
		~Texture() {};
		GLuint get() { return m_textureId; }
		void load(std::string _image);
		glm::vec3 m_col = glm::vec3(1.0f, 1.0f, 1.0f);
	
		int resolutionX, resolutionY, m_channels;
		GLuint m_textureId;		
	};
}

#endif