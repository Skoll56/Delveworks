#include "Surface.h"
#include "RenderTexture.h"
#include "Camera.h"

namespace Engine
{
	void Surface::initialize(std::shared_ptr<Camera> _cam)
	{
		m_RT = createRenderTexture();
		m_camera = _cam;
		_cam->setSurface(m_self.lock());
	}

	void Surface::update()
	{
		m_RT->Initialise(m_size.x, m_size.y); //Need to reset the resolution of the Render Texture if the size of the surfaces changes
	}

	std::shared_ptr<RenderTexture> Surface::createRenderTexture()
	{
		std::shared_ptr<RenderTexture> RT = std::make_shared<RenderTexture>();
		RT->Initialise(m_size.x, m_size.y);		
		return RT;
	}
	void Surface::onDestroy()
	{
		
	}
}