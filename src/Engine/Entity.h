#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <vector>
#include <memory>
#include <iostream>
#include "Engine.h"
#include "Collider.h"
#include "Console.h"


namespace Engine
{
	class Component;
	class Collider;
	class Core;
	class Transform;

	/** \brief An Entity is an Object in the scene. It has a Transform and contains Components.	*/
	class Entity
	{
		friend class Core;
		friend class PhysicsEventUser;


		public:

		/** \brief Adds a new Component to the Entity. The Args get used for that Component's OnInitialise function. */
		template <typename T, typename ... Args>
		std::shared_ptr<T> addComponent(Args&&... args)
		{
			try
			{
				if (!m_inAftertick)
				{
					std::shared_ptr<T> rtn = std::make_shared<T>();
					checkType(rtn);

					rtn->m_transform = m_transform;
					rtn->m_entity = m_self;
					rtn->m_self = rtn;
					rtn->onInitialise(std::forward<Args>(args)...);
					m_components.push_back(rtn);
					return rtn;
				}
				else
				{
					Console::output(Console::Error, "Add Component", "New components cannot be add while the Entity is in AfterTick phase as it conflicts with the delete phase. Try using onTick to add the component instead.");
					return nullptr;
				}

			}
			catch(Exception &e)
			{
				Console::output(Console::Error, "AddComponent", e.message());
				return nullptr;
			}			
		}

		/** \briefReturns a Component of type T, if the Entity has such a Component attached */
		template <typename T>
		std::shared_ptr<T> getComponent()
		{
			for (size_t i = 0; i < m_components.size(); i++)
			{
				std::shared_ptr<T> rtn = std::dynamic_pointer_cast<T>(m_components.at(i));
				if (rtn)
				{
					return rtn;
				}
			}	
			return nullptr;
		}
		
		/** \brief Returns the object's unique ID tag */
		std::string getTag() { return m_tag; }

		/** \brief Sets the object's unique ID tag*/
		void setTag(std::string _tag) { m_tag = _tag; }

		/** \brief Returns a reference to Core*/
		std::shared_ptr<Core> getCore() { return m_core.lock(); }

		/** \brief Returns whether or not an object is active*/
		bool isActive() { return m_active; }; //TODO: Make this work again

		/** \brief Enables or disables an object*/
		void setActive(bool _status) { m_active = _status; }

		/** \brief Returns the object's Transform*/
		std::shared_ptr<Transform> transform() { return m_transform; }
		
		/** \brief Schedules an object to be deleted at the end of the frame*/
		void destroy() { m_delete = true; }

	private:
		/** \brief Checks to see if a Component is a specific type. Used to pass references to Core*/
		template <typename T>
		void checkType(std::shared_ptr<T> _rtn)
		{
			bool foundType = false;
			std::shared_ptr<Transform> t = std::dynamic_pointer_cast<Transform>(_rtn);
			if (t)
			{
				foundType = true;
				m_transform = t;
			}
			if (!foundType)
			{
				std::shared_ptr<DirLight> d = std::dynamic_pointer_cast<DirLight>(_rtn);
				if (d)
				{
					foundType = true;					
					m_core.lock()->m_dirLights.push_back(d);
				}
			}
			if (!foundType)
			{
				std::shared_ptr<SpotLight> s = std::dynamic_pointer_cast<SpotLight>(_rtn);
				if (s)
				{
					foundType = true;
					m_core.lock()->m_spotLights.push_back(s);
				}
			}
			if (!foundType)
			{
				std::shared_ptr<PointLight> p = std::dynamic_pointer_cast<PointLight>(_rtn);
				if (p)
				{
					foundType = true;
					m_core.lock()->m_pointLights.push_back(p);
				}
			}			
			if (!foundType)
			{
				std::shared_ptr<AudioReceiver> a = std::dynamic_pointer_cast<AudioReceiver>(_rtn);
				if (a)
				{
					foundType = true;
					m_core.lock()->m_listener = a;
				}
			}			
		}

		/** \brief A list of all the Components on this Entity*/
		std::vector<std::shared_ptr<Component>> m_components;	

		/** \brief A reference to Core*/
		std::weak_ptr<Core> m_core;

		/** \brief A "This" pointer*/
		std::weak_ptr<Entity> m_self;

		/** \brief This object's unique ID tag*/
		std::string m_tag;

		/** \brief This object's transform*/
		std::shared_ptr<Transform> m_transform;	

		/** \brief An active object doesn't get updated*/
		bool m_active = true;

		/** \brief If true, the object will be deleted at the end of the frame*/
		bool m_delete = false;

		/** \brief Marks if the Entity is in "Aftertick" phase.*/
		bool m_inAftertick = false;	

		/** \brief Gets called once per frame for all active entities*/
		void tick();

		/** \brief Gets called once per frame for all active entities, after tick.*/
		void afterTick();

		/** \brief Gets called when a Collision ends*/
		void onCollisionExit(std::shared_ptr<Entity> _c);

		/** \brief Gets called for every frame with a Collision event*/
		void onCollision(std::shared_ptr<Collision> _c);
		/** \brief This gets called when the object collides for the first frame*/
		void onCollisionEnter(std::shared_ptr<Collision> _c);
	};
}

#endif