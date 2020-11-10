#ifndef COMPONENT_H
#define COMPONENT_H
#include <memory>
#include <glm/glm.hpp>
#include <AL/al.h>
#include "Resource.h"

namespace Engine
{
	class Entity;
	class Component
	{
		friend class Entity;

	public:
		void destroy();
	protected:
		virtual void onInitialise();
		virtual void onTick();
		virtual void onRender();
		virtual void onDestroy();
		std::weak_ptr<Entity> m_entity;

		bool m_delete = false;
	};

	class Transform : public Component
	{
	private:
		glm::vec3 m_position;
		glm::vec3 m_eulerAngles;
		glm::vec3 m_scale;
	};

	class SoundSource : public Component
	{
		void onInitialise(std::shared_ptr<Sound> _sound);
		void OnTick();
		void Play();

		ALuint m_id = 0;
		bool m_played = false;
		bool m_loop = false;
		bool m_destroyOnPlay = false;
	};
	
}

#endif
