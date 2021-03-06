#include "RenderTexture.h"
#include "Texture.h"
#include "Exception.h"

namespace Engine
{
	/* !This file has been CREATED as part of the GRAPHICS UNIT! */

	void RenderTexture::Initialise()
	{
		m_resX = 1024;
		m_resY = 1024;
		glGenFramebuffers(1, &m_fBufID);
		if (!m_fBufID)
		{
			throw Exception("Failed to generate framebuffer (Render Texture)");
		}
		glBindFramebuffer(GL_FRAMEBUFFER, m_fBufID);
		
		glGenTextures(1, &m_textureId);
		if (!m_textureId)
		{
			throw Exception("Failed to generate textureID (Render Texture)");
		}
		glBindTexture(GL_TEXTURE_2D, m_textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_resX, m_resY, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureId, 0);


		glGenRenderbuffers(1, &m_rBufObjID);
		if (!m_rBufObjID)
		{
			throw Exception("Failed to generate buffer object (Render Texture)");
		}
		glBindRenderbuffer(GL_RENDERBUFFER, m_rBufObjID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_resX, m_resY);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rBufObjID);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			throw Exception("Failed to generate Render Texture");
		}		
	}

	void RenderTexture::Initialise(int _resX, int _resY)
	{
		m_resX = _resX;
		m_resY = _resY;
		glGenFramebuffers(1, &m_fBufID);
		if (!m_fBufID)
		{
			throw Exception("Failed to generate framebuffer (Render Texture)");
		}
		glBindFramebuffer(GL_FRAMEBUFFER, m_fBufID);

		glGenTextures(1, &m_textureId);
		if (!m_textureId)
		{
			throw Exception("Failed to generate textureID (Render Texture)");
		}
		glBindTexture(GL_TEXTURE_2D, m_textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_resX, m_resY, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureId, 0);


		glGenRenderbuffers(1, &m_rBufObjID); 
		if (!m_rBufObjID)
		{
			throw Exception("Failed to generate buffer object (Render Texture)");
		}
		glBindRenderbuffer(GL_RENDERBUFFER, m_rBufObjID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, m_resX, m_resY);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rBufObjID);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			throw Exception("Failed to generate Render Texture");
		}
	}

	void ShadowMap::Initialise(float _xres, float _yres)
	{
		m_resX = _xres;
		m_resY = _yres;
		glGenFramebuffers(1, &m_fBufID);
		if (!m_fBufID)
		{
			throw Exception("Failed to generate framebuffer (Shadowmap)");
		}
		

		glGenTextures(1, &m_textureId);
		if (!m_textureId)
		{
			throw Exception("Failed to generate textureID (Shadowmap)");
		}
		glBindTexture(GL_TEXTURE_2D, m_textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_resX, m_resY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL); //
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		float bc[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bc);


		glBindFramebuffer(GL_FRAMEBUFFER, m_fBufID);		
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_textureId, 0);//
		//glDrawBuffer(GL_NONE);
		//glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	
	void ShadowCube::Initialise()
	{
		
		m_resX = 1024;
		m_resY = 1024;
	
		glGenFramebuffers(1, &m_fBufID);
		if (!m_fBufID)
		{
			throw Exception("Failed to generate framebuffer (Shadowcube)");
		}

		glGenTextures(1, &m_textureId);
		if (!m_textureId)
		{
			throw Exception("Failed to generate textureID (Shadowcube)");
		}
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureId);
		
		for (int i = 0; i < 6; i++)
		{			
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, m_resX, m_resY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL); 
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindFramebuffer(GL_FRAMEBUFFER, m_fBufID);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP, m_textureId, 0);//
		//glDrawBuffer(GL_NONE);
		//glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glBindTexture(GL_TEXTURE_2D, 0);
		m_lSpaceMatrix.resize(6);
	}
}
