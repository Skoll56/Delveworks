#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <vector>
#include <memory>
#include <iostream>
#include "Engine.h"
#include "Collider.h"


namespace Engine
{
	struct Component;
	class Collider;
	struct Core;
	class Transform;

	struct Entity
	{
		template <typename T>
		std::shared_ptr<T> addComponent()
		{
			std::shared_ptr<T> rtn = std::make_shared<T>();
			std::shared_ptr<Transform> t = std::dynamic_pointer_cast<Transform>(rtn);
			if (t) { m_transform = t; }
			components.push_back(rtn);			
			rtn->m_entity = self;
			return rtn;
		}
		void tick();

		template <typename T>
		std::shared_ptr<T> getComponent()
		{
			for (size_t i = 0; i < components.size(); i++)
			{
				std::shared_ptr<T> rtn = std::dynamic_pointer_cast<T>(components.at(i));
				if (rtn)
				{
					return rtn;
				}
			}
			std::cout << "Component not found" << std::endl;
			return nullptr;
		}

		
		std::shared_ptr<Collider> getCollider();
		

		std::string getTag() { return m_tag; }
		void setTag(std::string _tag) { m_tag = _tag; }
		std::shared_ptr<Core> getCore() { return core.lock(); }
		bool isActive() { return m_active; };
		void setActive(bool _status) { m_active = _status; }
		std::shared_ptr<Transform> transform() { return m_transform; }

	private:
		std::vector<std::shared_ptr<Component>> components;		
		std::weak_ptr<Core> core;
		std::weak_ptr<Entity> self;
		std::string m_tag;
		std::shared_ptr<Transform> m_transform;
		bool m_active;
	};
}

#endif