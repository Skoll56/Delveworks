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
	//test->transform()->m_eulerAngles = glm::vec3(90.0f, 0.0f, 0.0f);

	/*std::shared_ptr<Entity> sun = core->createEntity();
	std::shared_ptr<DirLight> d = sun->addComponent<DirLight>();
	d->setValues(glm::vec3(0.5f, 0.5f, 0.5f), 0.4f, glm::vec3(0.05f, 0.05f, 0.05f));
	d->transform()->m_position = glm::vec3(0.0f, 80.0f, 0.0f);
	d->transform()->m_eulerAngles = glm::vec3(90.0f, 0.0f, 0.0f);*/

	std::shared_ptr<Entity> point = core->createEntity();
	std::shared_ptr<PointLight> p = point->addComponent<PointLight>();
	p->setValues(glm::vec3(1.0f, 1.0f, 1.0f), 0.8f, 30.0f, 3.0f);
	p->transform()->m_position = glm::vec3(0.0f, 16.0f, 8.0f);
	p->transform()->m_eulerAngles = glm::vec3(90.0f, 0.0f, 0.0f);

	/*std::shared_ptr<Entity> point2 = core->createEntity();
	std::shared_ptr<PointLight> p2 = point2->addComponent<PointLight>();
	p2->setValues(glm::vec3(1.0f, 1.0f, 1.0f), 0.8f, 30.0f, 3.0f);
	p2->transform()->m_position = glm::vec3(0.0f, 16.0f, -8.0f);
	p2->transform()->m_eulerAngles = glm::vec3(90.0f, 0.0f, 0.0f);*/

	std::shared_ptr<Entity> spot2 = core->createEntity();
	std::shared_ptr<SpotLight> s2 = spot2->addComponent<SpotLight>();
	s2->setValues(glm::vec3(1.0f, 1.0f, 1.0f), 0.8f, 20.0f, 35.0f, 30.0f, 3.0f);
	s2->transform()->m_position = glm::vec3(10.0f, 15.0f, 0.0f);
	s2->transform()->m_eulerAngles = glm::vec3(90.01f - 65.0f, 0.0f, 0.0f);
	

	//s->transform()->m_eulerAngles = core->m_camera->transform()->m_eulerAngles;
	//s->transform()->m_position = core->m_camera->transform()->m_position;
	
	
	std::shared_ptr<Entity> floor = core->createEntity();
	std::shared_ptr<MeshRenderer> MR0 = floor->addComponent<MeshRenderer>(); //floor
	MR0->Initialise("diffuse.bmp", "1b1cube.obj", glm::vec3(1.0f, 1.0f, 1.0f));
	floor->transform()->setScale(glm::vec3(30.0f, 1.0f, 30.0f));
	std::shared_ptr<BoxCollider> b = floor->addComponent<BoxCollider>();

	std::shared_ptr<Entity> wall1 = core->createEntity();
	std::shared_ptr<MeshRenderer> MR1 = wall1->addComponent<MeshRenderer>();
	MR1->Initialise("diffuse.bmp", "1b1cube.obj", glm::vec3(1.0f, 1.0f, 1.0f));
	wall1->transform()->setScale(glm::vec3(30.0f, 20.0f, 1.0f)); //Forward
	wall1->transform()->m_position = glm::vec3(0.0f, 10.0f, 15.0f);
	std::shared_ptr<BoxCollider> b1 = wall1->addComponent<BoxCollider>();

	std::shared_ptr<Entity> wall2 = core->createEntity();
	std::shared_ptr<MeshRenderer> MR2 = wall2->addComponent<MeshRenderer>();
	MR2->Initialise("diffuse.bmp", "1b1cube.obj", glm::vec3(1.0f, 1.0f, 1.0f));
	wall2->transform()->setScale(glm::vec3(1.0f, 20.0f, 30.0f)); //Left
	wall2->transform()->m_position = glm::vec3(-15.0f, 10.0f, 0.0f);
	std::shared_ptr<BoxCollider> b2 = wall2->addComponent<BoxCollider>();

	std::shared_ptr<Entity> wall3 = core->createEntity();
	std::shared_ptr<MeshRenderer> MR3 = wall3->addComponent<MeshRenderer>();
	MR3->Initialise("diffuse.bmp", "1b1cube.obj", glm::vec3(1.0f, 1.0f, 1.0f));
	wall3->transform()->setScale(glm::vec3(1.0f, 20.0f, 30.0f));  //Right
	wall3->transform()->m_position = glm::vec3(15.0f, 10.0f, 0.0f);
	std::shared_ptr<BoxCollider> b3 = wall3 ->addComponent<BoxCollider>();

	std::shared_ptr<Entity> wall4 = core->createEntity();
	std::shared_ptr<MeshRenderer> MR4 = wall4->addComponent<MeshRenderer>();
	MR4->Initialise("diffuse.bmp", "1b1cube.obj", glm::vec3(1.0f, 1.0f, 1.0f));
	wall4->transform()->setScale(glm::vec3(30.0f, 20.0f, 1.0f)); //Back
	wall4->transform()->m_position = glm::vec3(0.0f, 10.0f, -15.0f);
	std::shared_ptr<BoxCollider> b4 = wall4->addComponent<BoxCollider>();

	std::shared_ptr<Entity> wall5 = core->createEntity();
	std::shared_ptr<MeshRenderer> MR5 = wall5->addComponent<MeshRenderer>();
	MR5->Initialise("diffuse.bmp", "1b1cube.obj", glm::vec3(1.0f, 1.0f, 1.0f));
	wall5->transform()->setScale(glm::vec3(30.0f, 1.0f, 30.0f)); //Roof
	wall5->transform()->m_position = glm::vec3(0.0f, 20.0f, 0.0f);
	std::shared_ptr<BoxCollider> b5 = wall5->addComponent<BoxCollider>();
	
	
	for (int i = 0; i < 6; i++)
	{
		std::shared_ptr<Entity> ball = core->createEntity();
		std::shared_ptr<MeshRenderer> MR3 = ball->addComponent<MeshRenderer>();
		MR3->Initialise("Image1.bmp", "1b1sphere.obj", glm::vec3(1.0f, 1.0f, 1.0f));
		ball->transform()->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
		ball->transform()->setPosition(glm::vec3(0.0f + (i * 0.2f), 13.0f + (i * 1.5f), 10.0f));
		std::shared_ptr<SphereCollider> sc = ball->addComponent<SphereCollider>();
		std::shared_ptr<AdvPhysicsObject> phys = ball->addComponent<AdvPhysicsObject>();
		phys->Initialise(1.0f, 0.9f);
	}	

	core->start();
	return 0;
}