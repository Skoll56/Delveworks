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

	/** \brief  The UI Object Base Class.*/
	/** This is the base class for any UI element. All UI is a "Surface" as it consumes space on the screen.*/	
	class Surface : public Component		
	{
		friend class Core;
		public:		
		void setPosition(glm::vec2 _pos);
		void setPosition(float _x, float _y);

		/** \brief 	This is an object's position in the context */
		glm::vec2 getPosition();

		/** \brief  Gets the true position of the surface*/
		/**	This is an object's true position in the window, rather than in the context */
		glm::vec2 getTruePosition();		
		glm::vec2 getSize() { return m_size; }		
		int getLayer() { return m_layer; }
		void setLayer(int _layer);						
		void setAlpha(float _a) { m_alpha = _a; }
		float getAlpha() { return m_alpha; }	

		/** \brief Sets the color overlay of the surface*/
		void setColor(glm::vec3 _RGB) { m_col = _RGB; }

		/** \brief Gets the color overlay of the surface*/
		glm::vec3 getColor() { return m_col; }

		/** \brief  Set whether an object scales with the window or not.	*/
		/**	This means the item will/won't be stretched when the Window is stretched.*/
		void setScaleWithContext(bool _tf) { scaleWithContext = _tf; } 

		/** \brief A "This" pointer*/
		std::weak_ptr<Surface> m_self;

		protected:			
		/** \brief 	This creates the quad to display the texture on.*/
		void createScreenQuad();

		/** \brief Transparency*/
		float m_alpha = 1.0f;

		/** \brief Layer determines the draw order. Higher layers draw over lower layers.*/
		int m_layer = 0;	
		glm::vec2 m_position;
		glm::vec2 m_size = glm::vec2(200, 200);

		/** \brief  This bool just marks if a surface is a RenderTexture or not (DisplayUI)*/
		bool isRender = false;
		bool scaleWithContext = true;

		/** \brief Color*/
		glm::vec3 m_col = glm::vec3(1.0f, 1.0f, 1.0f);
		
		/** \brief  A pointer to the Game Context*/
		std::weak_ptr<Context> m_context;		

		/** \brief The quad this object holds*/
		std::shared_ptr<VertexArray> m_screenQuad;		
	};

	/** \brief  A Display is any surface that a camera can render to*/
	/** Think of this like a 'Screen' which displays what a camera sees */
	class DisplayUI : public Surface
	{
		friend class Core;
		public:
		/** \brief Sets everything up*/
		void onInitialise(std::shared_ptr<Camera> _cam, int _layer);

		/** \brief Updates the RenderTexture when the Display size changes*/
		/** You probably won't need to call this manually, but it won't break if you do. */
		void update();
		void setSize(glm::vec2 _XY) { m_size = _XY; update(); }
		void setSize(int _X, int _Y) { m_size.x = _X; m_size.y = _Y; update(); }

		/** \brief Replaces the attached Camera with another one*/
		void setCamera(std::shared_ptr<Camera> _cam);

		/** \brief A "This" pointer*/
		std::weak_ptr<DisplayUI> m_self;

		private:
		/** \brief The attached Camera*/
		std::weak_ptr<Camera> m_camera;		

		/** \brief The attached RenderTexture*/
		std::shared_ptr<RenderTexture> m_RT;
		std::shared_ptr<RenderTexture> createRenderTexture();
	};

	class ImageUI : public Surface
	{
		friend class Core;
	public:

		/** \brief Sets everything up*/
		void onInitialise(std::shared_ptr<Texture> _tex, int _layer);
		void setSize(glm::vec2 _XY) { m_size = _XY; }
		void setSize(int _X, int _Y) { m_size.x = _X; m_size.y = _Y; }

		/** \brief A "This" pointer*/
		std::weak_ptr<ImageUI> m_self;

	private:
		/** \brief The Texture being displayed on the Surface*/
		std::shared_ptr<Texture> m_tex;
	};

	/** \brief  A Button
	/** This is a UI button, just an image with added functionality. The button responds only to Mouse events, this does not interact with a Controller */
	class ButtonUI : public ImageUI
	{
	public:
		/** \brief Update*/
		void onTick();

		/** \brief Returns True for every frame the button is pressed*/
		bool isButtonHeld() { return m_buttonHeld; }

		/** \brief Returns True on the frame the button is released*/
		bool buttonUp() { return m_buttonUp; }

		/** \brief Returns True on the frame the button is pressed*/
		bool isButtonDown() { return m_buttonDown; }

		/** \brief Replaces the default click button (Ie: left/right click)*/
		void setInteractButton(Mouse::MouseButton _button) { m_mouseButton = _button; }

		/** \brief Sets what color the button becomes when the mouse is over it*/
		void setMouseOverColor(glm::vec3 _rgb) { m_mouseOverColor = _rgb; }

		/** \brief Sets what color the button becomes when clicked*/
		void setClickColor(glm::vec3 _rgb) { m_clickedColor = _rgb; }

		/** \brief Returns True for every frame the cursor is over the button */
		bool isMouseOver() { return m_mouseOver; }

	private:
		/** \brief A reference to the Mouse*/
		std::weak_ptr<Mouse> m_mouse;
		bool m_buttonDown = false;
		bool m_buttonUp = false;
		bool m_buttonHeld = false;
		bool m_mouseOver = false;

		/** \brief The MouseButton which triggers the click*/
		Mouse::MouseButton m_mouseButton = Mouse::Left;

		/** \brief The default color of the button*/
		glm::vec3 m_normalColor = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 m_mouseOverColor = glm::vec3(0.7f, 0.7f, 0.7f);
		glm::vec3 m_clickedColor = glm::vec3(0.4f, 0.4f, 0.4f);		
	};

	/** \brief  A Context is a Window within a Window*/
	/** The context is largely used by the Engine, but the user can read its values. This is the 'Game Window' within the SDL Window. The Engine's UI is also a different Context.*/
	class Context
	{
		friend class Core;
		public:		
		glm::vec2 getPosition() { return m_position; }
		glm::vec2 getSize() { return m_size; }

		private:
		void setPosition(glm::vec2 _pos) { m_position = _pos; }
		void setPosition(float _x, float _y) { m_position.x = _x; m_position.y = _y; }

		/** \brief The list of surfaces in the Game Context*/
		std::vector<std::weak_ptr<Surface>> m_surfaces;

		/** \brief The list of surfaces in the Engine Context*/
		std::vector<std::shared_ptr<Surface>> m_engineSurfaces;
		glm::vec2 m_position;
		glm::vec2 m_size = glm::vec2(1024, 1024);
	};

}