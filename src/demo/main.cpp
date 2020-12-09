#include <iostream>
#include <Engine/Engine.h>
#define OUTPUT(vec) std::cout << vec.x << " " << vec.y << " " << vec.z <<std::endl;

#undef main
int main()
{	
	std::cout << "We started!" << std::endl;
	std::shared_ptr<Core> core = Core::initialise();

	std::shared_ptr<Entity> test = core->createEntity();
	std::shared_ptr<MeshRenderer> MR = test->addComponent<MeshRenderer>();
	MR->Initialise("statue_diffuse.png", "statue.obj", glm::vec3(5.0f, 10.0f, 5.0f));

	/*std::shared_ptr<Entity> sun = core->createEntity();
	std::shared_ptr<DirLight> d = sun->addComponent<DirLight>();
	d->setValues(glm::vec3(0.5f, 0.5f, 0.5f), 0.4f, glm::vec3(0.05f, 0.05f, 0.05f));
	d->transform()->m_position = glm::vec3(0.0f, 20.0f, 0.0f);
	d->transform()->m_eulerAngles = glm::vec3(90.0f, 0.0f, 0.0f);*/

	std::shared_ptr<Entity> spot = core->createEntity();
	std::shared_ptr<SpotLight> s = spot->addComponent<SpotLight>();
	s->setValues(glm::vec3(1.0f, 1.0f, 1.0f), 0.8f, 20.0f, 35.0f, 30.0f, 3.0f);
	s->transform()->m_position = glm::vec3(-10.0f, 15.0f, 10.0f);
	s->transform()->m_eulerAngles = glm::vec3(155.01f, 0.0f, 0.0f);

	std::shared_ptr<Entity> spot2 = core->createEntity();
	std::shared_ptr<SpotLight> s2 = spot2->addComponent<SpotLight>();
	s2->setValues(glm::vec3(1.0f, 1.0f, 1.0f), 0.8f, 20.0f, 35.0f, 30.0f, 3.0f);
	s2->transform()->m_position = glm::vec3(10.0f, 15.0f, 10.0f);
	s2->transform()->m_eulerAngles = glm::vec3(90.01f - 65.0f, 0.0f, 0.0f);
	

	//s->transform()->m_eulerAngles = core->m_camera->transform()->m_eulerAngles;
	//s->transform()->m_position = core->m_camera->transform()->m_position;
	
	
	std::shared_ptr<Entity> floor = core->createEntity();
	std::shared_ptr<MeshRenderer> MR2 = floor->addComponent<MeshRenderer>();
	MR2->Initialise("diffuse.bmp", "1b1cube.obj", glm::vec3(1.0f, 1.0f, 1.0f));
	floor->transform()->setScale(glm::vec3(30.0f, 1.0f, 30.0f));
	std::shared_ptr<BoxCollider> p = floor->addComponent<BoxCollider>();
	
	
	for (int i = 0; i < 6; i++)
	{
		std::shared_ptr<Entity> ball = core->createEntity();
		std::shared_ptr<MeshRenderer> MR3 = ball->addComponent<MeshRenderer>();
		MR3->Initialise("Image1.bmp", "1b1sphere.obj", glm::vec3(1.0f, 1.0f, 1.0f));
		ball->transform()->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
		ball->transform()->setPosition(glm::vec3(0.0f + (i * 0.2f), 8.0f + (i * 1.5f), 8.0f));
		std::shared_ptr<SphereCollider> sc = ball->addComponent<SphereCollider>();
		std::shared_ptr<AdvPhysicsObject> phys = ball->addComponent<AdvPhysicsObject>();
		phys->Initialise(1.0f, 0.9f);
	}	

	core->start();
	return 0;
}