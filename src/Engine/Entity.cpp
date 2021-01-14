#include "Entity.h"
#include "Component.h"
#include "Console.h"

namespace Engine
{
	void Entity::tick()
	{
		for (int i = 0; i < m_components.size(); i++)
		{
			try
			{
				m_components[i]->onTick();
			}
			catch(Exception &e)
			{
				Console::output(Console::Error, "Entity Tick", e.message());
			}
		}
	}

	void Entity::afterTick()
	{
		m_inAftertick = true;
		for (std::vector<std::shared_ptr<Component>>::iterator it = m_components.begin(); it != m_components.end();)
		{
			try
			{
				if ((*it)->m_delete || m_delete)
				{					
					{
						(*it)->onDestroy();
						it = m_components.erase(it);
					}
				}
				else
				{
					std::shared_ptr<PhysicsEventUser> p = std::dynamic_pointer_cast<PhysicsEventUser>((*it));
					if (p)
					{
						p->handleCollisions();
					}
					it++;
				}
			}
			catch (Exception &e)
			{
				Console::output(Console::Error, "Entity AfterTick", e.message());
				it++;
			}
		}
		m_inAftertick = false;
	}
	

	void Entity::onCollisionEnter(std::shared_ptr<Collision> _c)
	{
		for (std::vector<std::shared_ptr<Component>>::iterator it = m_components.begin(); it != m_components.end(); it++)
		{
			try
			{
				(*it)->onCollisionEnter(_c);
			}
			catch (Exception &e)
			{
				Console::output(Console::Error, "Entity OnCollisionEnter", e.message());
			}
		}
	}

	void Entity::onCollisionExit(std::shared_ptr<Entity> _c)
	{
		for (std::vector<std::shared_ptr<Component>>::iterator it = m_components.begin(); it != m_components.end(); it++)
		{
			try
			{
				(*it)->onCollisionExit(_c);
			}
			catch (Exception &e)
			{
				Console::output(Console::Error, "Entity OnCollisionExit", e.message());
			}
		}
	}

	void Entity::onCollision(std::shared_ptr<Collision> _c)
	{
		for (std::vector<std::shared_ptr<Component>>::iterator it = m_components.begin(); it != m_components.end(); it++)
		{
			try
			{
				(*it)->onCollision(_c);
			}
			catch (Exception &e)
			{
				Console::output(Console::Error, "Entity OnCollision", e.message());
			}			
		}
	}
}

