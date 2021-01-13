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

	/** \brief  A texture which can display from the view of a Camera*/
	/** Used either as a DisplayUI or as an object's Texture */
	class RenderTexture : public Texture
	{
		public:
		/** \brief Initialise (Default resolution 1024x1024)*/
		virtual void Initialise();
		/** \brief Initialise (specify resolution)*/
		virtual void Initialise(int _resX, int _resY);

		/** \brief Get the Framebuffer ID*/
		GLuint getFrameBufID() { return m_fBufID; }

		/** \brief Get the RenderBuffer ID*/
		GLuint getRenderBufID() { return m_rBufObjID; }
		
		protected:
		/** \brief ID of the frame buffer*/
		GLuint m_fBufID = 0;		

		/** \brief ID of the Render Buffer*/
		GLuint m_rBufObjID = 0;
	};

	/* !This has been CREATED as part of the GRAPHICS UNIT! */
	/** \brief A ShadowMap is a Render Texture which draws depth values instead of colour*/
	/** Used for Light sources for shadowcasting.  */
	class ShadowMap : public RenderTexture
	{
		public:
		/** \brief Initialise with a resolution*/
		void Initialise(float _xres, float _yres);

		/** \brief Returns the Light Space Matrix */
		glm::mat4 getMatrix() { return m_lSpaceMatrix; }

		/** \brief Sets the Light Space Matrix*/
		void setMatrix(glm::mat4 _lSpaceMatrix) { m_lSpaceMatrix = _lSpaceMatrix; }

		private:
		/** \brief The Light space matrix is the same as a "View" matrix for the Camera. It represents the position and direction of the Light.*/
		glm::mat4 m_lSpaceMatrix;
	};

	
	/* !This has been CREATED as part of the GRAPHICS UNIT! */
	/** \brief A ShadowCube is very similar to six ShadowMap objects arranged into a 3D cube*/
	/** This is for all-direction shadowcasting, which PointLight objects must do.  */
	class ShadowCube : public RenderTexture
	{
		public:
		/** \brief Initialise*/
		void Initialise();
		/** \brief 	Get one of the Light Space matrices*/
		glm::mat4 getMatrix(int _i) { return m_lSpaceMatrix[_i]; }
		/** \brief  Set a light space matrix*/
		void setMatrix(int _i, glm::mat4 _mat) { m_lSpaceMatrix[_i] = _mat; }

		private:
		/** \brief This contains 6 light space matrices, one for each direction*/
		std::vector<glm::mat4> m_lSpaceMatrix;
	};
}

#endif