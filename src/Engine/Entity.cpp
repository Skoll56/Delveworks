#include "Entity.h"
#include "Component.h"


namespace Engine
{
	void Entity::tick()
	{
		for (size_t ci = 0; ci < components.size(); ci++)
		{
			if (components[ci]->m_delete)
			{
				components[ci]->onDestroy(); //TODO: Destroy the object properly
			}
			else
			{
				components[ci]->onTick();
			}
		}
	}

	std::shared_ptr<Collider> Entity::getCollider() 
	{
		for (size_t i = 0; i < components.size(); i++)
		{
			std::shared_ptr<SphereCollider> rtn1 = std::dynamic_pointer_cast<SphereCollider>(components.at(i));
			if (rtn1)
			{
				return rtn1;
			}
			std::shared_ptr<MeshCollider> rtn2 = std::dynamic_pointer_cast<MeshCollider>(components.at(i));
			if (rtn2)
			{
				return rtn2;
			}
			std::shared_ptr<PlaneCollider> rtn3 = std::dynamic_pointer_cast<PlaneCollider>(components.at(i));
			if (rtn3)
			{
				return rtn3;
			}
			std::shared_ptr<BoxCollider> rtn4 = std::dynamic_pointer_cast<BoxCollider>(components.at(i));
			if (rtn4)
			{
				return rtn4;
			}
		}
		//std::cout << "Component not found" << std::endl;
		return nullptr;
	}
}

