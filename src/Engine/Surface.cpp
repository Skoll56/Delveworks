#include "Surface.h"
#include "RenderTexture.h"
#include "Camera.h"
#include "VertexBuffer.h"
#include "VertexArray.h"

namespace Engine
{
	void Surface::initialize(std::shared_ptr<Camera> _cam, int _layer)
	{
		m_RT = createRenderTexture();
		createScreenQuad();
		m_camera = _cam;
		m_layer = _layer;
		_cam->setSurface(m_self.lock());
	}

	void Surface::initialize(std::shared_ptr<Texture> _tex, int _layer)
	{
		m_tex = _tex;
		createScreenQuad();
		m_layer = _layer;
	}

	void Surface::update()
	{		
		if (m_RT)
		{
			m_RT->Initialise(m_size.x, m_size.y); //Need to reset the resolution of the Render Texture if the size of the surfaces changes
		}		
	}

	std::shared_ptr<RenderTexture> Surface::createRenderTexture()
	{
		std::shared_ptr<RenderTexture> RT = std::make_shared<RenderTexture>();
		RT->Initialise(m_size.x, m_size.y);		
		return RT;
	}
	void Surface::setCamera(std::shared_ptr<Camera> _cam)
	{
		m_camera = _cam;
	}
	void Surface::setLayer(int _layer)
	{
		m_layer = _layer;
		m_core.lock()->orderSurfaces();
	}

	/* !This has been CREATED as part of the GRAPHICS UNIT! */
	/*And later modified as part of the Surface system for Game Engine Unit*/
	void Surface::createScreenQuad()
	{
		m_screenQuad = std::make_shared<VertexArray>();
		m_screenQuad->setBuffer("in_Position", new VertexBuffer());
		m_screenQuad->setBuffer("in_TexCoord", new VertexBuffer());

		m_screenQuad->getTriPos()->add(glm::vec3(-1.0f, -1.0f, 0.0f));
		m_screenQuad->getTriPos()->add(glm::vec3(1.0f, -1.0f, 0.0f));
		m_screenQuad->getTriPos()->add(glm::vec3(1.0f, 1.0f, 0.0f));
		m_screenQuad->getTriPos()->add(glm::vec3(1.0f, 1.0f, 0.0f));
		m_screenQuad->getTriPos()->add(glm::vec3(-1.0f, 1.0f, 0.0f));
		m_screenQuad->getTriPos()->add(glm::vec3(-1.0f, -1.0f, 0.0f));

		m_screenQuad->getTriTex()->add(glm::vec2(0.0f, 0.0f));
		m_screenQuad->getTriTex()->add(glm::vec2(1.0f, 0.0f));
		m_screenQuad->getTriTex()->add(glm::vec2(1.0f, 1.0f));
		m_screenQuad->getTriTex()->add(glm::vec2(1.0f, 1.0f));
		m_screenQuad->getTriTex()->add(glm::vec2(0.0f, 1.0f));
		m_screenQuad->getTriTex()->add(glm::vec2(0.0f, 0.0f));
	}

	void Surface::onDestroy()
	{
		
	}
}