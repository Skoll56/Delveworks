#include "Surface.h"
#include "RenderTexture.h"
#include "Camera.h"
#include "VertexBuffer.h"
#include "VertexArray.h"

namespace Engine
{
	void DisplayUI::onInitialise(std::shared_ptr<Camera> _cam, int _layer)
	{		
		isRender = true;
		m_RT = createRenderTexture();
		createScreenQuad();
		m_camera = _cam;
		m_layer = _layer;
		

		_cam->addSurface(m_self.lock());
		getCore()->addDisplay(m_self.lock());
	}

	void ImageUI::onInitialise(std::shared_ptr<Texture> _tex, int _layer)
	{
		m_tex = _tex;		
		createScreenQuad();
		m_layer = _layer;
		getCore()->addUISurface(m_self.lock());
	}

	void DisplayUI::update()
	{		
		if (m_RT)
		{
			m_RT->Initialise(m_size.x, m_size.y); //Need to reset the resolution of the Render Texture if the size of the surfaces changes
		}		
	}

	std::shared_ptr<RenderTexture> DisplayUI::createRenderTexture()
	{
		std::shared_ptr<RenderTexture> RT = std::make_shared<RenderTexture>();
		RT->Initialise(m_size.x, m_size.y);		
		return RT;
	}
	void DisplayUI::setCamera(std::shared_ptr<Camera> _cam)
	{
		m_camera = _cam;
	}
	void Surface::setPosition(glm::vec2 _pos)
	{
		m_position = _pos + m_context.lock()->getPosition();
	}
	void Surface::setPosition(float _x, float _y)
	{
		m_position = glm::vec2(_x, _y) + m_context.lock()->getPosition();
	}
	glm::vec2 Surface::getPosition()
	{
		return m_position - m_context.lock()->getPosition();
	}
	glm::vec2 Surface::getTruePosition()
	{
		return m_position; 
	}

	void Surface::setLayer(int _layer)
	{
		m_layer = _layer;
		getCore()->orderSurfaces();
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

	void ButtonUI::onInitialise(std::shared_ptr<Texture> _tex, int _layer)
	{
		m_tex = _tex;
		createScreenQuad();
		m_layer = _layer;
		getCore()->addUISurface(m_self.lock());

		m_touch = getInputDevice<Touchscreen>();
		m_mouse = getInputDevice<Mouse>();
	}

	void ButtonUI::onTick()
	{
		m_mouseOver = false;
		m_buttonUp = false;
		m_buttonDown = false;

		if (!m_mouse.lock())		
		{
			m_mouse = getInputDevice<Mouse>();
		}
		else
		{
			glm::vec2 mousePos = m_mouse.lock()->getTruePosition();
			glm::vec2 myPos = getTruePosition();
			if (mousePos.x >= myPos.x && mousePos.y >= myPos.y)
			{
				if (mousePos.x <= myPos.x + m_size.x && mousePos.y <= myPos.y + m_size.y)
				{
					m_mouseOver = true;
					if (m_mouse.lock()->getButtonDown(m_mouseButton))
					{
						m_buttonDown = true;
						m_buttonHeld = true;
					}
					else if (m_mouse.lock()->getButtonUp(m_mouseButton))
					{
						m_buttonHeld = false;
						m_buttonUp = true;
					}
				}
				else
				{
					m_buttonHeld = false;
				}
			}
			else
			{
				m_buttonHeld = false;
			}

			if (m_mouseOver && !m_buttonHeld)
			{
				m_col = m_mouseOverColor;//
			}
			else if (m_buttonHeld)
			{
				m_col = m_clickedColor;
			}
			else
			{
				m_col = m_normalColor;
			}
		}

		if (!m_touch.lock())
		{
			m_touch = getInputDevice<Touchscreen>();
		}
		else
		{			
			glm::vec2 touchPos = m_touch.lock()->getTruePosition();
			glm::vec2 myPos = getTruePosition();
			if (touchPos.x >= myPos.x && touchPos.y >= myPos.y)
			{
				if (touchPos.x <= myPos.x + m_size.x && touchPos.y <= myPos.y + m_size.y)
				{
					m_mouseOver = true;
					if (m_touch.lock()->getFingerDown())
					{
						m_buttonDown = true;
						m_buttonHeld = true;
					}
					else if (m_touch.lock()->getFingerUp())
					{
						m_buttonHeld = false;
						m_buttonUp = true;
					}
				}
				else
				{
					m_buttonHeld = false;
				}
			}
			else
			{
				m_buttonHeld = false;
			}

			if (m_buttonHeld)
			{
				m_col = m_clickedColor;
			}
			else
			{
				m_col = m_normalColor;
			}
			
		}
	}
}