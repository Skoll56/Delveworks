#include "Entity.h"
#include "Component.h"
#include "Console.h"

namespace Engine
{
	void Entity::tick()
	{
		for (std::vector<std::shared_ptr<Component>>::iterator it = components.begin(); it != components.end(); it++)
		{
			try
			{
				(*it)->onTick();
			}
			catch(Exception &e)
			{
				Console::output(Console::Error, "Entity Tick", e.message());
			}
		}
	}

	void Entity::afterTick()
	{
		for (std::vector<std::shared_ptr<Component>>::iterator it = components.begin(); it != components.end();)
		{
			try
			{
				if ((*it)->m_delete || m_delete)
				{
					std::shared_ptr<Camera> cam = std::dynamic_pointer_cast<Camera>((*it));
					if (cam)
					{
						if (cam->getSurface())
						{
							m_delete = false;
							(*it)->m_delete = false;
							it++;
							Console::output(Console::Error, "Component Delete", "A camera cannot be deleted with a surface still attached. Consider deleting the surface first");
						}		
						else
						{
							(*it)->onDestroy();
							it = components.erase(it);
						}
					}
					else
					{
						(*it)->onDestroy();
						it = components.erase(it);
					}
				}
				else
				{
					std::shared_ptr<PhysicsObject> p = std::dynamic_pointer_cast<PhysicsObject>((*it));
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
	}

	std::shared_ptr<Collider> Entity::getCollider() 
	{
		for (std::vector<std::shared_ptr<Component>>::iterator it = components.begin(); it != components.end(); it++)
		{
			std::shared_ptr<SphereCollider> rtn1 = std::dynamic_pointer_cast<SphereCollider>((*it));
			if (rtn1)
			{
				return rtn1;
			}
			std::shared_ptr<MeshCollider> rtn2 = std::dynamic_pointer_cast<MeshCollider>((*it));
			if (rtn2)
			{
				return rtn2;
			}
			std::shared_ptr<PlaneCollider> rtn3 = std::dynamic_pointer_cast<PlaneCollider>((*it));
			if (rtn3)
			{
				return rtn3;
			}
			std::shared_ptr<BoxCollider> rtn4 = std::dynamic_pointer_cast<BoxCollider>((*it));
			if (rtn4)
			{
				return rtn4;
			}
		}		
		return nullptr;
	}

	void Entity::onCollisionEnter(std::shared_ptr<Collision> _c)
	{
		for (std::vector<std::shared_ptr<Component>>::iterator it = components.begin(); it != components.end(); it++)
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
		for (std::vector<std::shared_ptr<Component>>::iterator it = components.begin(); it != components.end(); it++)
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
		for (std::vector<std::shared_ptr<Component>>::iterator it = components.begin(); it != components.end(); it++)
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

