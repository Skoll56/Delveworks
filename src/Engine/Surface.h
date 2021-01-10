#include <glm/glm.hpp>
#include <memory>

namespace Engine 
{
	class RenderTexture;
	class Camera;
	class VertexArray;
	class Texture;
	class Core;
	
	class Surface		
	{
		friend class Core;
		public:
		void destroy() { m_destroy = true; }
		void setPosition(glm::vec2 _pos) { m_position = _pos;  }
		void setPosition(float _x, float _y) { m_position.x = _x; m_position.y = _y; }
		glm::vec2 getPosition() { return m_position; }
		
		glm::vec2 getSize() { return m_size; }
		
		int getLayer() { return m_layer; }
		void setLayer(int _layer);
		void createScreenQuad();
		void setAlpha(float _a) { m_alpha = _a; }
		float getAlpha() { return m_alpha; }

		protected:
		bool m_destroy = false;
		float m_alpha = 1.0f;
		int m_layer = 0;
		void onDestroy();
		glm::vec2 m_position;
		glm::vec2 m_size = glm::vec2(1024, 1024);
		bool isRender = false;
		
		
		
		std::weak_ptr<Core> m_core;
		std::shared_ptr<VertexArray> m_screenQuad;		
	};

	class RenderSurface : public Surface
	{
		friend class Core;
		public:
			void initialize(std::shared_ptr<Camera> _cam, int _layer);
			void update();
			void setSize(glm::vec2 _XY) { m_size = _XY; update(); }
			void setSize(int _X, int _Y) { m_size.x = _X; m_size.y = _Y; update(); }
			void setCamera(std::shared_ptr<Camera> _cam);

		private:
			std::weak_ptr<Camera> m_camera;
			std::weak_ptr<RenderSurface> m_self;
			std::shared_ptr<RenderTexture> m_RT;

			std::shared_ptr<RenderTexture> createRenderTexture();
	};

	class UISurface : public Surface
	{
		friend class Core;
	public:
		void initialize(std::shared_ptr<Texture>, int _layer);
		void setSize(glm::vec2 _XY) { m_size = _XY; }
		void setSize(int _X, int _Y) { m_size.x = _X; m_size.y = _Y; }
		std::weak_ptr<UISurface> m_self;

	private:
		std::shared_ptr<Texture> m_tex;
	};

}