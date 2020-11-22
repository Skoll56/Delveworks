#include <iostream>
#include <Engine/Engine.h>

#undef main
int main()
{	
	std::cout << "We started!" << std::endl;
	std::shared_ptr<Core> core = Core::initialise();

	std::shared_ptr<Entity> test = core->createEntity();
	std::shared_ptr<MeshRenderer> MR = test->addComponent<MeshRenderer>();
	MR->Initialise("statue_diffuse.png", "statue.obj", glm::vec3(5.0f, 10.0f, 5.0f));

	std::shared_ptr<Entity> sun = core->createEntity();
	std::shared_ptr<DirLight> dlight = sun->addComponent<DirLight>();
	dlight->setValues(glm::vec3(0.5f, 0.5f, 0.5f), 0.4f, glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.3f, -1.0f, 0.0f));
	
	std::shared_ptr<Entity> floor = core->createEntity();
	std::shared_ptr<MeshRenderer> MR2 = floor->addComponent<MeshRenderer>();
	MR2->Initialise("diffuse.bmp", "1b1plane.obj", glm::vec3(1.0f, 1.0f, 1.0f));
	floor->transform()->setScale(glm::vec3(100.0f, 1.0f, 100.0f));

	core->start();
	return 0;
}

//int OldMain()
//{
//	
//	
//}