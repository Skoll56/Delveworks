
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


	void Texture::addTexture(std::string _image)
	{
		std::cout << "Adding texture: " << _image << std::endl;
		m_w = 0;
		m_h = 0;
		m_channels = 0;
		_image = "resources/Textures/" + _image;
		std::cout << "About to load" << std::endl;
		unsigned char *data = stbi_load(_image.c_str(), &m_w, &m_h, &m_channels, 4);
		std::cout << "Ended load" << std::endl;
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
		std::cout << "Didn't break yet" << std::endl;
		glBindTexture(GL_TEXTURE_2D, m_textureId);
		std::cout << "1" << std::endl;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_w, m_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		std::cout << "2" << std::endl;
		free(data);
		std::cout << "3" << std::endl;
		glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "4" << std::endl;
		glBindTexture(GL_TEXTURE_2D, 0);
		std::cout << "5" << std::endl;
	}
}