#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Component.h"

namespace Engine
{
	class VertexArray;
	class VertexBuffer;
	class Texture;

	class MeshRenderer : public Component
	{
	public:
		void onTick();
		VertexArray * m_vAO; // The shape's mesh/obj
		VertexBuffer *m_vBOt; // The vertex buffer for texture co-ordinates
		VertexBuffer *m_vBOp; // The vertex buffer for object co-ordinates
		Texture *m_tex;

		
		void setShine(int _shininess) { m_shininess = _shininess; }
		int getShine() { return m_shininess; }
		glm::vec3 getSize() { return m_size; }
		

	private:
		glm::vec3 m_size; // The size of the obj in generic units (ie: box objs are 1/1/1 units)
		
		int m_shininess;
		
	};
}