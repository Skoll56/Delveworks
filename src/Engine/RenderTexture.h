#ifndef _RENDERTEXTURE_H_
#define _RENDERTEXTURE_H_

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/ext.hpp>
#include <exception>
#include "Texture.h"


namespace Engine
{
	/* !This has been CREATED as part of the GRAPHICS UNIT! */
	class RenderTexture : public Texture
	{
		public:
		virtual void Initialise();
			
		GLuint fBufID = 0;		
		GLuint rBufObjID = 0;
	};

	/* !This has been CREATED as part of the GRAPHICS UNIT! */
	class ShadowMap : public RenderTexture
	{
	public:
		void Initialise(float _xres, float _yres);
		glm::mat4 getLightSpaceMatrix() { return m_lSpaceMatrix; }
		void setLightSpaceMatrix(glm::mat4 _lSpaceMatrix) { m_lSpaceMatrix = _lSpaceMatrix; }

	private:
		glm::mat4 m_lSpaceMatrix;
	};

	/* !This has been CREATED as part of the GRAPHICS UNIT! */
	class ShadowCube : public RenderTexture
	{
	public:
		void Initialise();
		glm::mat4 getMatrix(int _i) { return m_lSpaceMatrix[_i]; }
		void setMatrix(int _i, glm::mat4 _mat) { m_lSpaceMatrix[_i] = _mat; }

	private:
		std::vector<glm::mat4> m_lSpaceMatrix;
	};
}

#endif