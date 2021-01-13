#ifndef _MESHRENDERER_H_
#define _MESHRENDERER_H_

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
	class Shader;

	/** \brief A Mesh Renderer displays an object's visuals to the screen*/
	/** The MeshRenderer is partly comprised of an object's Texture, VertexArray and VertexBuffer */
	class MeshRenderer : public Component
	{
		public:
		/** \brief Draws the Mesh using its triangle positions and textures*/
		void draw();

		/** \brief Initialise the mesh with a path to its Texture image, a path to its VertexArray (obj file) and the image size*/
		void onInitialise(std::string _texName, std::string _obj, glm::vec3 _size);

		/** \brief Set the shininess of the object's surface*/
		void setShine(int _shininess) { m_shininess = _shininess; }

		/** \brief Get the shininess of the object's surface*/
		int getShine() { return m_shininess; }

		/** \brief Get the size  of the mesh in generic units*/
		glm::vec3 getSize() { return m_size; }

		/** \brief Set the Alpha of the object*/
		void setAlpha(float _a) { m_alpha = _a; }

		/** \brief Get the alpha of the object*/
		float getAlpha() { return m_alpha; }

		/** \brief Set whether or not the object casts shadows*/
		void setCastShadows(bool _tf) { m_castShadows = _tf; }

		/** \brief Get whether or not the object casts shadows*/
		bool getCastShadows() { return m_castShadows; }

		/** \brief Sets whether the object receieves shadows*/
		void setReceiveShadows(bool _tf) { m_receiveShadows = _tf; }

		/** \brief Get whether the object receieves shadows*/
		bool getReceiveShadows() { return m_receiveShadows; }
		
		/** \brief Returns the VertexBuffer containing the Texture co-ordinates*/
		VertexBuffer* getTextureCoords() { return m_vBOt; }

		/** \brief Returns the VertexBuffer containing the OBJ's vertex co-ordinates*/
		VertexBuffer* getObjectCoords() { return m_vBOp; }

		/** \brief Returns the object's Texture*/
		std::shared_ptr<Texture> getTexture() { return m_tex; }

		/** \brief Returns the object's Shader*/
		std::shared_ptr<Shader> getShader() { return m_shader; }

		/** \brief Returns the Vertex array (obj)*/
		std::shared_ptr<VertexArray> getMesh() { return m_vAO; }

		/** \brief A "This" pointer*/
		std::weak_ptr<MeshRenderer> m_self;
		
		/** \brief Returns the color adjustment to the mesh*/
		glm::vec3 getColor() { return m_color; }

		/** \brief Returns the mesh's emissive light property */
		glm::vec3 getEmissive() { return m_lightEmission; }

		/** \brief Sets the mesh's emissive light property */
		void setEmissive(glm::vec3 _emissive) { m_lightEmission = _emissive; }




		private:
		/** \brief  The size of the obj in generic units (ie: box objs are 1/1/1 units)*/
		glm::vec3 m_size; 

		/** \brief The shininess multiplier for lighting calculations*/
		int m_shininess = 0;

		/** \brief  The shape's mesh/obj*/
		std::shared_ptr<VertexArray> m_vAO; 

		/** \brief The vertex buffer for texture co-ordinates*/
		VertexBuffer *m_vBOt; 

		/** \brief The vertex buffer for object co-ordinates*/
		VertexBuffer *m_vBOp; 

		/** \brief The Texture on the object*/
		std::shared_ptr<Texture> m_tex;

		/** \brief The shader the object is user*/
		std::shared_ptr<Shader> m_shader;

		/** \brief A color adjustment to the Mesh*/
		glm::vec3 m_color = glm::vec3(1.0f, 1.0f, 1.0f);

		/** \brief The transparency value of the object*/
		float m_alpha = 1.0f;

		/** \brief Mark if the object casts shadows or not*/
		bool m_castShadows = true;

		/** \brief Mark if the object emits light or not */
		glm::vec3 m_lightEmission = glm::vec3(0.0f);

		/** \brief Mark is shadows can be cast unto this object or not*/
		bool m_receiveShadows = true;
	};
}
#endif