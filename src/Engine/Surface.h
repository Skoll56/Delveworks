#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "Component.h"
#include "Input.h"

namespace Engine 
{
	class RenderTexture;
	class Camera;
	class VertexArray;
	class Texture;
	class Core;
	class Context;
	
	
	class Surface : public Component		
	{
		friend class Core;
		public:		
		void setPosition(glm::vec2 _pos);
		void setPosition(float _x, float _y);
		glm::vec2 getPosition();
		std::weak_ptr<Surface> m_self;
		glm::vec2 getTruePosition();
		
		glm::vec2 getSize() { return m_size; }
		
		int getLayer() { return m_layer; }
		void setLayer(int _layer);		
		void setAlpha(float _a) { m_alpha = _a; }
		float getAlpha() { return m_alpha; }
		void setScaleWithContext(bool _tf) { scaleWithContext = _tf; }

		protected:
		void createScreenQuad();		
		float m_alpha = 1.0f;
		int m_layer = 0;		
		glm::vec2 m_position;
		glm::vec2 m_size = glm::vec2(200, 200);
		bool isRender = false;
		bool scaleWithContext = true;
		glm::vec3 m_col = glm::vec3(1.0f, 1.0f, 1.0f);
		
		std::weak_ptr<Context> m_context;		
		std::shared_ptr<VertexArray> m_screenQuad;		
	};

	class DisplayUI : public Surface
	{
		friend class Core;
		public:
			void onInitialise(std::shared_ptr<Camera> _cam, int _layer);
			void update();
			void setSize(glm::vec2 _XY) { m_size = _XY; update(); }
			void setSize(int _X, int _Y) { m_size.x = _X; m_size.y = _Y; update(); }
			void setCamera(std::shared_ptr<Camera> _cam);
			std::weak_ptr<DisplayUI> m_self;

		private:
			std::weak_ptr<Camera> m_camera;			
			std::shared_ptr<RenderTexture> m_RT;
			std::shared_ptr<RenderTexture> createRenderTexture();
	};

	class ImageUI : public Surface
	{
		friend class Core;
	public:
		void onInitialise(std::shared_ptr<Texture> _tex, int _layer);
		void setSize(glm::vec2 _XY) { m_size = _XY; }
		void setSize(int _X, int _Y) { m_size.x = _X; m_size.y = _Y; }
		std::weak_ptr<ImageUI> m_self;

	private:
		std::shared_ptr<Texture> m_tex;
	};

	class ButtonUI : public ImageUI
	{
	public:

		void onTick();
		bool isButtonHeld() { return m_buttonHeld; }
		bool buttonUp() { return m_buttonUp; }
		bool isButtonDown() { return m_buttonDown; }
		void setInteractButton(Mouse::MouseButton _button) { m_mouseButton = _button; }
		void setMouseOverColor(glm::vec3 _rgb) { m_mouseOverColor = _rgb; }
		bool isMouseOver() { return m_mouseOver; }

	private:
		std::weak_ptr<Mouse> m_mouse;
		bool m_buttonDown = false;
		bool m_buttonUp = false;
		bool m_buttonHeld = false;
		bool m_mouseOver = false;
		Mouse::MouseButton m_mouseButton = Mouse::Left;
		glm::vec3 m_mouseOverColor = glm::vec3(0.7f, 0.7f, 0.7f);
		glm::vec3 m_clickedColor = glm::vec3(0.4f, 0.4f, 0.4f);
		glm::vec3 m_normalColor = glm::vec3(1.0f, 1.0f, 1.0f);
	};


	class Context
	{
		friend class Core;
		public:		
		glm::vec2 getPosition() { return m_position; }
		glm::vec2 getSize() { return m_size; }

		private:
		void setPosition(glm::vec2 _pos) { m_position = _pos; }
		void setPosition(float _x, float _y) { m_position.x = _x; m_position.y = _y; }
		std::vector<std::weak_ptr<Surface>> m_surfaces;
		std::vector<std::shared_ptr<Surface>> m_engineSurfaces;
		glm::vec2 m_position;
		glm::vec2 m_size = glm::vec2(1024, 1024);
	};

}