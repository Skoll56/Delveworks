#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>
#include <glm/glm.hpp>
#include <AL/al.h>
#include "Resource.h"
#include "Input.h"
#include "Core.h"
//#include "Entity.h"

namespace Engine
{
	class Entity;	
	class Transform;
	class Keyboard;
	class Collision;	
	class Collider;

	class Component
	{
		friend class Entity;

	public:
		void destroy();
		std::shared_ptr<Entity> getEntity() { return m_entity.lock(); }
		std::shared_ptr<Transform> transform() { return m_transform.lock(); }
		std::weak_ptr<Component> m_self;

		template <typename T>
		std::shared_ptr<T> getInputDevice()
		{
			return getCore()->getInput()->getDevice<T>();
		}



		std::shared_ptr<Core> getCore();

		/*template <typename T>
		std::shared_ptr<T> getComponent()
		{
			getEntity()->getComponent<T>();
		}*/


	protected:
		virtual void onInitialise();
		virtual void onCollision(std::shared_ptr<Collision> _col);
		virtual void onCollisionEnter(std::shared_ptr<Collision> _col);
		virtual void onCollisionExit(std::shared_ptr<Entity> _other);
		virtual void onTick();
		virtual void onRender();
		virtual void onDestroy();
		std::weak_ptr<Entity> m_entity;
		std::weak_ptr<Transform> m_transform;
		bool m_delete = false;
	};

	class Transform : public Component
	{
		friend class Entity;
	public:
		void setPosition(float _posX, float _posY, float _posZ) { m_position.x = _posX; m_position.y = _posY; m_position.z = _posZ; }
		void setPosition(glm::vec3 _value) { m_position = _value; }
		glm::vec3 getPosition() { return m_position; }
		glm::vec3 getScale() { return m_scale; }
		void setScale(glm::vec3 _scale) { m_scale = _scale; }
		glm::vec3 getSize() { return m_size; }
		glm::mat4 getModel() { return m_model; }
		void onTick();
		void setModel(glm::mat4 _model) { m_model = _model; }
		std::weak_ptr<Transform> m_self;
		void rotate(glm::vec3 _axis, float _amt) { m_eulerAngles += _amt * _axis; }
		glm::vec3 getUp() { return m_upVec; }
		glm::vec3 getRight() { return m_right; }
		glm::vec3 getFwd() { return m_fwd; }
		glm::vec3 m_position = glm::vec3(0.0f);
		glm::vec3 m_eulerAngles = glm::vec3(0.0f);
		

	private:		
		void onInitialise();
		glm::vec3 m_scale = glm::vec3(1.0f);
		glm::vec3 m_size = glm::vec3(1.0f);
		glm::mat4 m_model = glm::mat4(1.0f);
		glm::vec3 m_fwd;
		glm::vec3 m_upVec;
		glm::vec3 m_right;
	};

	class SoundSource : public Component
	{
		friend class Entity;
		public:
		
		void onTick();
		void Play(float _vol);
		bool m_loop = false;
		bool m_destroyOnPlay = false;
		std::weak_ptr<SoundSource> m_self;

		private:
		void onInitialise(std::shared_ptr<Sound> _sound);
		ALuint m_id = 0;
		bool m_played = false;
		
	};

	class AudioReceiver : public Component
	{
		public:
		std::weak_ptr<AudioReceiver> m_self;
	};
}

#endif
