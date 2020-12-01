#ifndef _RENDERTEXTURE_H_
#define _RENDERTEXTURE_H_

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/ext.hpp>
#include <exception>
#include "Texture.h"

//RENDER TEXTURES ARE A PART OF THE GRAPHICS UNIT AND WERE ADDED AS A FIRST STEP TOWARDS SHADOWS
namespace Engine
{
	class RenderTexture : public Texture
	{
		public:
		virtual void Initialise();
			
		GLuint fBufID = 0;		
		GLuint rBufObjID = 0;
	};

	class ShadowMap : public RenderTexture
	{
	public:
		void Initialise();
		glm::mat4 getLightSpaceMatrix() { return m_lSpaceMatrix; }
		void setLightSpaceMatrix(glm::mat4 _lSpaceMatrix) { m_lSpaceMatrix = _lSpaceMatrix; }

	private:
		glm::mat4 m_lSpaceMatrix;
	};
}

#endif