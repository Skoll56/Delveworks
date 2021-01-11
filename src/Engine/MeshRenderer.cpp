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
		m_shader->setUniform("in_color", m_col);
		m_shader->setUniform("in_alpha", m_alpha);
		m_shader->setUniform("in_rShadows", (int)receiveShadows);

		if (m_alpha < 1.0f)
		{
			glDisable(GL_DEPTH);
			m_shader->draw(m_vAO);
			glEnable(GL_DEPTH);
		}
		else
		{
			m_shader->draw(m_vAO);
		}
		
	}

	void MeshRenderer::onInitialise(std::string _texName, std::string _obj, glm::vec3 _size)
	{
		std::shared_ptr<ResourceManager> RM = getEntity()->getCore()->m_rManager;
		m_shader = RM->load<Shader>("light");
		m_vBOp = new VertexBuffer();
		m_vBOt = new VertexBuffer();		
		m_tex = RM->load<Texture>(_texName);
		m_vAO = RM->load<VertexArray>(_obj);
		m_tex->load(_texName);
		m_shininess = 1;
	}
}