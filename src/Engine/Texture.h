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

	private:
		int m_w, m_h, m_channels;
		GLuint m_textureId;
	};
}

#endif