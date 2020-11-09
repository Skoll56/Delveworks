#ifndef _Texture_H_
#define _Texture_H_
#include <GL/glew.h>

namespace Engine
{
	class Texture
	{
	public:
		Texture();
		~Texture() {};
		GLuint get() { return m_textureId; }
		void addTexture(std::string _image);

	private:
		int m_w, m_h, m_channels;
		GLuint m_textureId;
	};
}

#endif