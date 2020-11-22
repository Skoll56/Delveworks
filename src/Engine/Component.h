#ifndef COMPONENT_H
#define COMPONENT_H
#include <memory>
#include <glm/glm.hpp>
#include <AL/al.h>
#include "Resource.h"

namespace Engine
{
	class Entity;	
	class Transform;

	class Component
	{
		friend class Entity;

	public:
		void destroy();
		std::shared_ptr<Entity> getEntity() { return m_entity.lock(); }
		std::shared_ptr<Transform> transform() { return m_transform.lock(); }

	protected:
		virtual void onInitialise();
		virtual void onTick();
		virtual void onRender();
		virtual void onDestroy();
		std::weak_ptr<Entity> m_entity;	
		std::weak_ptr<Transform> m_transform;
		bool m_delete = false;
	};

	class Transform : public Component
	{
	public:
		void setPosition(float _posX, float _posY, float _posZ) { m_position.x = _posX; m_position.y = _posY; m_position.z = _posZ; }
		void setPosition(glm::vec3 _value) { m_position = _value; }
		glm::vec3 getPosition() { return m_position; }
		glm::vec3 getScale() { return m_scale; }
		glm::vec3 getSize() { return m_size; }
		glm::mat4 getModel() { return m_model; }
		void setModel(glm::mat4 _model) { m_model = _model; }

	private:
		glm::vec3 m_position;
		glm::vec3 m_eulerAngles;
		glm::vec3 m_scale;
		glm::vec3 m_size;
		glm::mat4 m_model;
	};

	class SoundSource : public Component
	{
		void onInitialise(std::shared_ptr<Sound> _sound);
		void onTick();
		void Play();

		ALuint m_id = 0;
		bool m_played = false;
		bool m_loop = false;
		bool m_destroyOnPlay = false;
	};
	
}

#endif
