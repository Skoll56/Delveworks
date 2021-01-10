#include <iostream>
#include <Engine/Engine.h>

class Ball : public Component
{
	std::weak_ptr<SoundSource> m_sound;
	
	public:
	void onInitialise()
	{		
		m_sound = getEntity()->addComponent<SoundSource>(getCore()->m_rManager->load<Sound>("test2.ogg"));
		//m_sound = getEntity()->addComponent<SoundSource>(getCore()->m_rManager->load<Sound>("pewTest.wav"));
	}

	void onCollisionExit(std::shared_ptr<Entity> _other)
	{
		if (!m_sound.lock()) throw Exception();
		m_sound.lock()->Play(0.8f);
		//Console::message("Collision happened");
	}
};

class Demo : public Component
{
	public:
	std::weak_ptr<Transform> camera;
	std::weak_ptr<Keyboard> keyboard;	
	std::weak_ptr<Mouse> mouse;	
	std::weak_ptr<Controller> con;	
	float moveSpeed = 10.0f;
	float rotSpeed = 10.0f;
	bool controller = false;
	int i = 0;
	

	void onInitialise()
	{
		camera = getCore()->getDefaultCamera()->transform();
		camera.lock()->getEntity()->addComponent<AudioReceiver>();

		std::shared_ptr<InputManager> IM = getCore()->getInput();
		mouse = IM->addDevice<Mouse>();
		keyboard = IM->addDevice<Keyboard>();
		con = IM->addDevice<Controller>();

		mouse.lock()->hideCursor(false);
		mouse.lock()->lockCursor(false);
	}

	void onTick()
	{
		float dTime = getCore()->getDeltaTime();
		i++;
		if (!controller)
		{
			if (keyboard.lock()->GetKeyIsDown(SDLK_w))
			{
				camera.lock()->m_position += camera.lock()->getFwd() * moveSpeed * dTime;
			}
			if (keyboard.lock()->GetKeyIsDown(SDLK_s))
			{
				camera.lock()->m_position -= camera.lock()->getFwd() * moveSpeed * dTime;
			}
			if (keyboard.lock()->GetKeyIsDown(SDLK_d))
			{
				camera.lock()->m_position -= camera.lock()->getRight() * moveSpeed * dTime;
			}
			if (keyboard.lock()->GetKeyIsDown(SDLK_a))
			{
				camera.lock()->m_position += camera.lock()->getRight() * moveSpeed * dTime;
			}

			camera.lock()->rotate(glm::vec3(0.0f, 1.0f, 0.0f), mouse.lock()->getDeltaPos().x * rotSpeed * dTime);
			camera.lock()->rotate(glm::vec3(1.0f, 0.0f, 0.0f), mouse.lock()->getDeltaPos().y * rotSpeed * dTime);
		}
		else
		{
			camera.lock()->m_position -= camera.lock()->getFwd() * moveSpeed * con.lock()->getLeftStickPosition().y * dTime;
			camera.lock()->m_position -= camera.lock()->getRight() * moveSpeed * con.lock()->getLeftStickPosition().x * dTime;

			camera.lock()->rotate(glm::vec3(0.0f, 1.0f, 0.0f), con.lock()->getRightStickPosition().x * rotSpeed * 10.0f * dTime);
			camera.lock()->rotate(glm::vec3(1.0f, 0.0f, 0.0f), con.lock()->getRightStickPosition().y * rotSpeed * 10.0f * dTime);

			if (con.lock()->getButtonIsDown(Controller::LeftStick))
			{
				Console::message("Yes, Controller works, too!");
			}
		}

		if (camera.lock()->m_eulerAngles.x > 89.0f) { camera.lock()->m_eulerAngles.x = 89.0f; }
		else if (camera.lock()->m_eulerAngles.x < -89.0f) { camera.lock()->m_eulerAngles.x = -89.0f; }
		if (camera.lock()->m_eulerAngles.y > 360.0f) { camera.lock()->m_eulerAngles.y = 0.0f; }
		else if (camera.lock()->m_eulerAngles.y < -360.0f) { camera.lock()->m_eulerAngles.y = 0.0f; }


		bool iWantToTest = false;
		if (iWantToTest)
		{
			if (i == 500)
			{
				std::shared_ptr<Camera> s = camera.lock()->getEntity()->getComponent<Camera>(); // ->getSurface();
				s->destroy();
				//s->setSize(glm::vec2(600, 600));
			}
			else if (i == 1000)
			{
				std::shared_ptr<Entity> e = getCore()->createEntity();
				std::shared_ptr<Camera> c = e->addComponent<Camera>();
				getCore()->setDefaultCamera(c);
			}
		}
	}
};

class UI : public Component
{
	public:
	bool test = true;
	int i;
	std::weak_ptr<Transform> camera;
	
	void onInitialise()
	{
		camera = getCore()->getDefaultCamera()->transform();
	}

	void onTick()
	{
		i++;
		if (test)
		{
			if (i == 120)
			{
				std::shared_ptr<Texture> t = getCore()->m_rManager->load<Texture>("defaultUI.bmp");
				t->m_col = glm::vec3(0.7f, 0.7f, 0.7f);
				std::shared_ptr<UISurface> surf = getCore()->createUISurface(t, 1);
				surf->setSize(400, 400);
				surf->setPosition(100, 100);


				//std::shared_ptr<RenderSurface> surf2 = getCore()->createRenderSurface(camera.lock()->getEntity()->getComponent<Camera>(), 2);
				////std::shared_ptr<UISurface> surf2 = getCore()->createUISurface(t, 2);
				//surf2->setSize(400, 400);
				//surf2->setAlpha(0.5f);
				//surf2->setPosition(300, 300);				
			}
		}
	}
};

class CustomInput : public InputDevice
{
	void update(std::vector<SDL_Event> _eventList)
	{
		for (int i = 0; i < _eventList.size(); i++)
		{
			if (_eventList[i].type == SDL_USEREVENT)
			{
				//Do stuff, or whatever. This is just an example of a custom input module.
			}
		}
	}
};


#undef main
int main()
{
	std::shared_ptr<Core> core = Core::initialise();

	//Create the statue entity
	std::shared_ptr<Entity> test = core->createEntity();
	std::shared_ptr<MeshRenderer> MR = test->addComponent<MeshRenderer>("statue_diffuse.png", "statue.obj", glm::vec3(5.0f, 10.0f, 5.0f));
	MR->m_alpha = 0.5f;
	MR->castShadows = true;
	//test->addComponent<MeshCollider>();
	test->transform()->m_position = glm::vec3(0.0f, 1.0f, 5.0f);

	//Create a demo object
	std::shared_ptr<Entity> demo = core->createEntity();
	demo->addComponent<Demo>();
	demo->addComponent<UI>();


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
	}

	//Create a spotlight (in another room)
	/*std::shared_ptr<Entity> spot2 = core->createEntity();
	std::shared_ptr<SpotLight> s2 = spot2->addComponent<SpotLight>();
	s2->setValues(glm::vec3(1.0f, 1.0f, 1.0f), 0.8f, 20.0f, 35.0f, 30.0f, 3.0f);//
	s2->transform()->m_position = glm::vec3(60.0f, 16.0f, 8.0f);
	s2->transform()->m_eulerAngles = glm::vec3(90.0f, 0.0f, 8.0f);//
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
		MR5->m_alpha = 0.25f;
		MR5->receiveShadows = true;
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
			ball->transform()->setPosition(glm::vec3(5.0f + (i * 0.2f) + l, 33.0f + (i * 1.5f), 5.0f));
			std::shared_ptr<SphereCollider> sc = ball->addComponent<SphereCollider>();
			//std::shared_ptr<PhysicsObject> phys = ball->addComponent<PhysicsObject>(1.0f, 0.5f);
			std::shared_ptr<AdvPhysicsObject> phys = ball->addComponent<AdvPhysicsObject>(1.0f, 0.9f);
			ball->addComponent<Ball>();
		}
		core->start();
	}
	return 0;	
}