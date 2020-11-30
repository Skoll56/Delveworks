#include "MeshRenderer.h"
#include "Entity.h"
#include <glm/glm.hpp>
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Texture.h"

namespace Engine
{
	void MeshRenderer::draw()
	{
		m_shader->setUniform("in_Shininess", (int)pow(2, getShine()));
		m_shader->setUniform("in_Model", transform()->getModel()); // Translate the model matrix by camera position and stuff
		m_shader->setUniform("in_TransverseModel", glm::transpose(glm::inverse(transform()->getModel())));
		m_shader->setUniform("in_Texture", m_tex);
		m_shader->draw(m_vAO);
	}
	void MeshRenderer::Initialise(std::string _texName, std::string _obj, glm::vec3 _size)
	{		
		m_vBOp = new VertexBuffer();
		m_vBOt = new VertexBuffer();
		m_vAO = new VertexArray();
		m_tex = std::make_shared<Texture>();
		m_vAO->loadObj(_obj);
		m_tex->load(_texName);
		m_shininess = 1;
	}

	void MeshRenderer::onInitialise()
	{
		m_shader = getEntity()->getCore()->m_rManager->load<Shader>("light");
	}
}