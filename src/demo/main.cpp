#include <iostream>
#include <Engine/Engine.h>

struct Player : public Component
{
	int dummy;
};

#undef main
int main()
{	
	std::cout << "We started!" << std::endl;
	std::shared_ptr<Core> core = Core::initialise();
	core->start();

	std::shared_ptr<Entity> player = core->createEntity();
	std::shared_ptr<Component> pc = player->addComponent<Player>();	
	return 0;
}

//int OldMain()
//{
//	
//	
//}