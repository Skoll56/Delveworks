#include <glm/glm.hpp>
#include <memory>

namespace Engine 
{
	class RenderTexture;
	class Camera;
	
	class Surface		
	{
		friend class Core;
		public:
		void initialize(std::shared_ptr<Camera> _cam);
		void update();
		std::shared_ptr<RenderTexture> createRenderTexture();
		void destroy() { m_destroy = true; }
		void setPosition(glm::vec2 _pos) { m_position = _pos; }
		glm::vec2 getPosition() { return m_position; }
		void setSize(glm::vec2 _XY) { m_size = _XY; }
		glm::vec2 getSize() { return m_size; }

		private:
		bool m_destroy = false;
		void onDestroy();
		glm::vec2 m_position;
		glm::vec2 m_size = glm::vec2(1024, 1024);
		std::shared_ptr<RenderTexture> m_RT;
		std::weak_ptr<Camera> m_camera;
		std::weak_ptr<Surface> m_self;
	};
}