#include <iostream>
#include <Engine/Engine.h>
#define OUTPUT(vec) std::cout << vec.x << " " << vec.y << " " << vec.z <<std::endl;

class DebugObject : public Component
{
	void DebugObject::onTick()
	{
		OUTPUT(transform()->getPosition())
	}
};

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
	MR2->Initialise("diffuse.bmp", "1b1cube.obj", glm::vec3(1.0f, 1.0f, 1.0f));
	floor->transform()->setScale(glm::vec3(20.0f, 1.0f, 20.0f));
	std::shared_ptr<BoxCollider> p = floor->addComponent<BoxCollider>();
	//p->setNorm(glm::vec3(0.0f, 1.0f, 0.0f));
	
	for (int i = 0; i < 6; i++)
	{
		std::shared_ptr<Entity> ball = core->createEntity();
		std::shared_ptr<MeshRenderer> MR3 = ball->addComponent<MeshRenderer>();
		MR3->Initialise("Image1.bmp", "1b1sphere.obj", glm::vec3(1.0f, 1.0f, 1.0f));
		ball->transform()->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
		ball->transform()->setPosition(glm::vec3(0.0f + (i * 0.2f), 8.0f + (i * 1.5f), 8.0f));
		std::shared_ptr<SphereCollider> s = ball->addComponent<SphereCollider>();
		std::shared_ptr<AdvPhysicsObject> phys = ball->addComponent<AdvPhysicsObject>();
		phys->Initialise(1.0f, 0.9f);
	}	

	core->start();
	return 0;
}

//int OldMain()
//{
//	
//	
//}