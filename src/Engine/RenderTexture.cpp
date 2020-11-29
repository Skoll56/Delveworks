#include "RenderTexture.h"
#include "Texture.h"

namespace Engine
{
	void RenderTexture::Initialise()
	{
		glGenFramebuffers(1, &fBufID);
		if (!fBufID)
		{
			throw std::exception();
		}
		glBindFramebuffer(GL_FRAMEBUFFER, fBufID);
		
		glGenTextures(1, &fBufTexID);
		if (!fBufTexID)
		{
			throw std::exception();
		}
		glBindTexture(GL_TEXTURE_2D, fBufTexID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, resolutionX, resolutionY, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fBufTexID, 0);


		glGenRenderbuffers(1, &rBufObjID);
		if (!rBufObjID)
		{
			throw std::exception();
		}
		glBindRenderbuffer(GL_RENDERBUFFER, rBufObjID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, resolutionX, resolutionY);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rBufObjID);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			throw std::exception();
		}		
	}

	void ShadowMap::Initialise()
	{
		glGenFramebuffers(1, &fBufID);
		if (!fBufID)
		{
			throw std::exception();
		}
		

		glGenTextures(1, &fBufTexID);
		if (!fBufTexID)
		{
			throw std::exception();
		}
		glBindTexture(GL_TEXTURE_2D, fBufTexID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, resolutionX, resolutionY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL); //
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBindFramebuffer(GL_FRAMEBUFFER, fBufID);		
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fBufTexID, 0);//
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);


		/*glGenRenderbuffers(1, &rBufObjID);
		if (!rBufObjID)
		{
			throw std::exception();
		}
		glBindRenderbuffer(GL_RENDERBUFFER, rBufObjID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, resolutionX, resolutionY);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rBufObjID);

		

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			throw std::exception();
		}*/
	}
}
