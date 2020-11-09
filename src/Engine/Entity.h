#include <vector>
#include <memory>
#include <iostream>


namespace Engine
{
	struct Component;
	struct Core;

	struct Entity
	{
		std::weak_ptr<Core> core;
		std::weak_ptr<Entity> self;

		template <typename T>
		std::shared_ptr<T> addComponent()
		{
			std::shared_ptr<T> rtn = std::make_shared<T>();
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
			return null;
		}




	private:
		std::vector<std::shared_ptr<Component>> components;			
	};
}