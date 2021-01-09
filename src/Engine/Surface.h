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
		void initialize(std::shared_ptr<Camera> _cam, int _layer);
		void initialize(std::shared_ptr<Texture>, int _layer);
		void update();
		std::shared_ptr<RenderTexture> createRenderTexture();
		void destroy() { m_destroy = true; }
		void setPosition(glm::vec2 _pos) { m_position = _pos; }
		glm::vec2 getPosition() { return m_position; }
		void setSize(glm::vec2 _XY) { m_size = _XY; update(); }
		void setSize(int _X, int _Y) { m_size.x = _X; m_size.y = _Y; update(); }
		glm::vec2 getSize() { return m_size; }
		void setCamera(std::shared_ptr<Camera> _cam);
		int getLayer() { return m_layer; }
		void setLayer(int _layer);
		void createScreenQuad();

		private:
		bool m_destroy = false;
		int m_layer = 0;
		void onDestroy();
		glm::vec2 m_position;
		glm::vec2 m_size = glm::vec2(1024, 1024);
		std::shared_ptr<RenderTexture> m_RT;
		std::shared_ptr<Texture> m_tex;
		std::weak_ptr<Camera> m_camera;
		std::weak_ptr<Surface> m_self;
		std::weak_ptr<Core> m_core;
		std::shared_ptr<VertexArray> m_screenQuad;
		
	};
}