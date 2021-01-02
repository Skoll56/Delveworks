#include "Entity.h"
#include "Component.h"


namespace Engine
{
	void Entity::tick()
	{
		for (std::vector<std::shared_ptr<Component>>::iterator it = components.begin(); it != components.end(); it++)
		{
			(*it)->onTick();			
		}
	}

	void Entity::afterTick()
	{
		for (std::vector<std::shared_ptr<Component>>::iterator it = components.begin(); it != components.end();)
		{
			if ((*it)->m_delete || m_delete)
			{
				(*it)->onDestroy(); 
				it = components.erase(it);
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
		//std::cout << "Component not found" << std::endl;
		return nullptr;
	}

	void Entity::onCollision(std::shared_ptr<Collision> _c)
	{
		for (std::vector<std::shared_ptr<Component>>::iterator it = components.begin(); it != components.end(); it++)
		{
			(*it)->onCollision(_c);
		}
	}
}

