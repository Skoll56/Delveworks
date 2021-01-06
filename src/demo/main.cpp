#include <iostream>
#include <Engine/Engine.h>

class Ball : public Component
{
	std::weak_ptr<SoundSource> m_sound;
	std::vector<int> t;
	int i = 0;

	public:
	void onInitialise()
	{		
		m_sound = getEntity()->addComponent<SoundSource>(getEntity()->getCore()->m_rManager->load<Sound>("pew"));
	}

	void onCollision(std::shared_ptr<Collision> _col)
	{	
		
	}

	void onCollisionExit(std::shared_ptr<Entity> _other)
	{
		
	}

	void onCollisionEnter(std::shared_ptr<Collision> _col)
	{
		if (!m_sound.lock()) throw Exception();
		m_sound.lock()->Play(0.2f);
		Console::message("Collision happened");
	}

	void onTick()
	{
		
	}
};


#undef main
int main()
{			
	std::shared_ptr<Core> core = Core::initialise();

	//Create the statue entity
	std::shared_ptr<Entity> test = core->createEntity();
	std::shared_ptr<MeshRenderer> MR = test->addComponent<MeshRenderer>("statue_diffuse.png", "statue.obj", glm::vec3(5.0f, 10.0f, 5.0f));
	//test->addComponent<MeshCollider>();
	test->transform()->m_position = glm::vec3(0.0f, 1.0f, 5.0f);

	//Create the directional (and ambient) light
	std::shared_ptr<Entity> sun = core->createEntity();
	std::shared_ptr<DirLight> d = sun->addComponent<DirLight>(glm::vec3(0.5f, 0.5f, 0.5f), 0.4f, glm::vec3(0.05f, 0.05f, 0.05f));

	d->transform()->m_position = glm::vec3(0.0f, 100.0f, 0.0f);
	d->transform()->m_eulerAngles = glm::vec3(90.0f, 0.0f, 0.0f);

	//Create i pointlights
	for (int i = 0; i < 1; i++)
	{
		std::shared_ptr<Entity> point = core->createEntity();
		std::shared_ptr<PointLight> p = point->addComponent<PointLight>(glm::vec3(1.0f, 1.0f, 1.0f), 0.8f, 30.0f, 0.8f);

		p->transform()->m_position = glm::vec3(0.0f, 6.0f, -2.0f);
		p->transform()->m_eulerAngles = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	//Create a spotlight (in another room)
	/*std::shared_ptr<Entity> spot2 = core->createEntity();
	std::shared_ptr<SpotLight> s2 = spot2->addComponent<SpotLight>();
	s2->setValues(glm::vec3(1.0f, 1.0f, 1.0f), 0.8f, 20.0f, 35.0f, 30.0f, 3.0f);
	s2->transform()->m_position = glm::vec3(60.0f, 16.0f, 8.0f);
	s2->transform()->m_eulerAngles = glm::vec3(90.0f, 0.0f, 8.0f);
	*/
	//This creates three rooms all 50 units away from eachother.
	for (float l = 0.0f; l < 50.0f; l += 50.0f)
	{
		std::shared_ptr<Entity> floor = core->createEntity();
		std::shared_ptr<MeshRenderer> MR0 = floor->addComponent<MeshRenderer>("diffuse.bmp", "1b1cube.obj", glm::vec3(1.0f, 1.0f, 1.0f)); //floor
		//MR0->Initialise();
		floor->transform()->m_position = glm::vec3(0.0f + l, 0.0f, 0.0f);
		floor->transform()->setScale(glm::vec3(30.0f, 0.1f, 30.0f));
		std::shared_ptr<PlaneCollider> b = floor->addComponent<PlaneCollider>();
		//b->m_trigger = true;
		b->setNorm(glm::vec3(0.0f, 1.0f, 0.0f));


		std::shared_ptr<Entity> wall1 = core->createEntity();
		std::shared_ptr<MeshRenderer> MR1 = wall1->addComponent<MeshRenderer>("diffuse.bmp", "1b1cube.obj", glm::vec3(1.0f, 1.0f, 1.0f));
		//MR1->Initialise();
		wall1->transform()->setScale(glm::vec3(30.0f, 20.0f, 1.0f)); //Forward
		wall1->transform()->m_position = glm::vec3(0.0f + l, 10.0f, 15.0f);
		std::shared_ptr<BoxCollider> b1 = wall1->addComponent<BoxCollider>();

		std::shared_ptr<Entity> wall2 = core->createEntity();
		std::shared_ptr<MeshRenderer> MR2 = wall2->addComponent<MeshRenderer>("diffuse.bmp", "1b1cube.obj", glm::vec3(1.0f, 1.0f, 1.0f));
		//MR2->Initialise();
		wall2->transform()->setScale(glm::vec3(1.0f, 20.0f, 30.0f)); //Left
		wall2->transform()->m_position = glm::vec3(-15.0f + l, 10.0f, 0.0f);
		std::shared_ptr<BoxCollider> b2 = wall2->addComponent<BoxCollider>();

		std::shared_ptr<Entity> wall3 = core->createEntity();
		std::shared_ptr<MeshRenderer> MR3 = wall3->addComponent<MeshRenderer>("diffuse.bmp", "1b1cube.obj", glm::vec3(1.0f, 1.0f, 1.0f));
		//MR3->Initialise();
		wall3->transform()->setScale(glm::vec3(1.0f, 20.0f, 30.0f));  //Right
		wall3->transform()->m_position = glm::vec3(15.0f + l, 10.0f, 0.0f);
		std::shared_ptr<BoxCollider> b3 = wall3->addComponent<BoxCollider>();

		std::shared_ptr<Entity> wall4 = core->createEntity();
		std::shared_ptr<MeshRenderer> MR4 = wall4->addComponent<MeshRenderer>("diffuse.bmp", "1b1cube.obj", glm::vec3(1.0f, 1.0f, 1.0f));
		//MR4->Initialise();
		wall4->transform()->setScale(glm::vec3(30.0f, 20.0f, 1.0f)); //Back
		wall4->transform()->m_position = glm::vec3(0.0f + l, 10.0f, -15.0f);
		std::shared_ptr<BoxCollider> b4 = wall4->addComponent<BoxCollider>();

		std::shared_ptr<Entity> wall5 = core->createEntity();
		std::shared_ptr<MeshRenderer> MR5 = wall5->addComponent<MeshRenderer>("diffuse.bmp", "1b1cube.obj", glm::vec3(1.0f, 1.0f, 1.0f));
		//MR5->Initialise();
		wall5->transform()->setScale(glm::vec3(30.0f, 1.0f, 30.0f)); //Roof
		wall5->transform()->m_position = glm::vec3(0.0f + l, 20.0f, 0.0f);
		std::shared_ptr<BoxCollider> b5 = wall5->addComponent<BoxCollider>();

		//Create bouncy balls (with WiP physics) to see
		for (int i = 0; i < 1; i++)
		{
			std::shared_ptr<Entity> ball = core->createEntity();
			std::shared_ptr<MeshRenderer> MR3 = ball->addComponent<MeshRenderer>("Image1.bmp", "1b1sphere.obj", glm::vec3(1.0f, 1.0f, 1.0f));
			//MR3->Initialise();
			ball->transform()->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
			ball->transform()->setPosition(glm::vec3(5.0f + (i * 0.2f) + l, 13.0f + (i * 1.5f), 5.0f));
			std::shared_ptr<SphereCollider> sc = ball->addComponent<SphereCollider>();
			//std::shared_ptr<PhysicsObject> phys = ball->addComponent<PhysicsObject>(1.0f, 0.5f);
			std::shared_ptr<AdvPhysicsObject> phys = ball->addComponent<AdvPhysicsObject>(1.0f, 0.9f);
			ball->addComponent<Ball>();
		}	
		core->start();
	}
	return 0;
}