
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include "stb_image.h"
#include "Texture.h"

namespace Engine
{
	Texture::Texture()
	{

	}


	void Texture::load(std::string _image)
	{		
		resolutionX = 0;
		resolutionY = 0;
		m_channels = 0;
		_image = "../resources/Textures/" + _image;		
		unsigned char *data = stbi_load(_image.c_str(), &resolutionX, &resolutionY, &m_channels, 4);		
		if (!data) 
		{ 
			std::cout << "Broke here" << std::endl;
			throw std::exception(); 
		}
		m_textureId = 0;
		glGenTextures(1, &m_textureId);
		if (!m_textureId) 
		{ 
			std::cout << "Broke here2" << std::endl;
			throw std::exception(); 
		}	
		glBindTexture(GL_TEXTURE_2D, m_textureId);		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, resolutionX, resolutionY, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);		
		free(data);		
		glGenerateMipmap(GL_TEXTURE_2D);		
		glBindTexture(GL_TEXTURE_2D, 0);		
	}
}