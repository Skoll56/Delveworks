#include <iostream>
#include <Engine/Engine.h>
#ifdef _WIN32
#include <Windows.h>
#endif

class Ball : public Component
{
	std::weak_ptr<SoundSource> m_sound;
	
	public:
	void onInitialise()
	{		
		m_sound = getEntity()->addComponent<SoundSource>(getCore()->load<Sound>("pew.ogg"));
	}

	void onCollisionExit(std::shared_ptr<Entity> _other)
	{
		if (!m_sound.lock()) throw Exception();
		m_sound.lock()->Play(0.8f);
	}
};

class Cthulhu : public Component
{
	public:
	void onTick()
	{
		transform()->rotate(glm::vec3(0, 1, 0), 90.0f * getCore()->getDeltaTime());
	}

	void onCollisionEnter(std::shared_ptr<Collision> _col)
	{
		Console::output(Console::Message, "C'thulhu", "HEY! Stop doing that!", true);
	}
};



class Demo : public Component
{
	public:
	std::weak_ptr<Camera> camera;	
	std::weak_ptr<Keyboard> keyboard;	
	std::weak_ptr<Mouse> mouse;	
	std::vector<std::weak_ptr<Entity>> balls;
	std::weak_ptr<ButtonUI> button;
	bool lockedCursor = true;
	
	float moveSpeed = 20.0f;
	float rotSpeed = 20.0f;
	
	int i = 0;
	
	void buildScene()
	{
		std::shared_ptr<Core> core = getCore();

		//Create the directional (and ambient) light
		for (int i = 0; i < 1; i++)
		{			
			std::shared_ptr<Entity> sun = core->createEntity();
			std::shared_ptr<DirLight> d = sun->addComponent<DirLight>(glm::vec3(0.5f, 0.5f, 0.5f), 0.4f, glm::vec3(0.05f, 0.05f, 0.05f));
			d->transform()->m_position = glm::vec3(0.0f, 100.0f, 0.0f);
			d->transform()->m_eulerAngles = glm::vec3(90.0f, 0.0f, 0.0f);
		}

		//Create i pointlights
		for (int i = 0; i < 1; i++)
		{
			std::shared_ptr<Entity> point = core->createEntity();
			std::shared_ptr<PointLight> p = point->addComponent<PointLight>(glm::vec3(1.0f, 1.0f, 1.0f), 0.8f, 30.0f, 0.8f);
			p->transform()->m_position = glm::vec3(0.0f, 6.0f, -2.0f);
		}

		for (int i = 0; i < 1; i++)
		{
			//Create i spotlight s
			std::shared_ptr<Entity> spot2 = core->createEntity();
			std::shared_ptr<SpotLight> s2 = spot2->addComponent<SpotLight>(glm::vec3(1.0f, 1.0f, 1.0f), 0.8f, 20.0f, 35.0f, 30.0f, 3.0f);
			s2->transform()->m_position = glm::vec3(0.0f, 16.0f, 8.0f);
			s2->transform()->m_eulerAngles = glm::vec3(90.0f, 0.0f, 8.0f);//
		}

		//Create the statue entity
		std::shared_ptr<Entity> test = core->createEntity();
		std::shared_ptr<MeshRenderer> MR = test->addComponent<MeshRenderer>("statue_diffuse.png", "statue.obj", glm::vec3(5.0f, 10.0f, 5.0f));
		test->addComponent<MeshCollider>();
		test->addComponent<Cthulhu>();
		std::shared_ptr<SphereCollider> sc = test->addComponent<SphereCollider>();
		test->addComponent<PhysicsEventUser>();
		sc->m_trigger = true;
		MR->setCastShadows(false);
		MR->setAlpha(0.9f);
		test->transform()->m_position = glm::vec3(0.0f, 5.0f, 0.0f);

		//This creates a room		
		std::shared_ptr<Entity> floor = core->createEntity();
		std::shared_ptr<MeshRenderer> MR0 = floor->addComponent<MeshRenderer>("diffuse.bmp", "1b1cube.obj", glm::vec3(1.0f, 1.0f, 1.0f)); //floor		
		floor->transform()->m_position = glm::vec3(0.0f, 0.5f, 0.0f);
		floor->transform()->setScale(glm::vec3(30.0f, 0.1f, 30.0f));
		std::shared_ptr<BoxCollider> b = floor->addComponent<BoxCollider>();

		std::shared_ptr<Entity> wall1 = core->createEntity();
		std::shared_ptr<MeshRenderer> MR1 = wall1->addComponent<MeshRenderer>("diffuse.bmp", "1b1cube.obj", glm::vec3(1.0f, 1.0f, 1.0f));
		
		wall1->transform()->setScale(glm::vec3(30.0f, 20.0f, 1.0f)); //Forward
		wall1->transform()->m_position = glm::vec3(0.0f, 10.0f, 14.0f);
		std::shared_ptr<BoxCollider> b1 = wall1->addComponent<BoxCollider>();

		std::shared_ptr<Entity> wall2 = core->createEntity();
		std::shared_ptr<MeshRenderer> MR2 = wall2->addComponent<MeshRenderer>("diffuse.bmp", "1b1cube.obj", glm::vec3(1.0f, 1.0f, 1.0f));
		
		wall2->transform()->setScale(glm::vec3(1.0f, 20.0f, 30.0f)); //Left
		wall2->transform()->m_position = glm::vec3(-14.0f, 10.0f, 0.0f);
		std::shared_ptr<BoxCollider> b2 = wall2->addComponent<BoxCollider>();

		std::shared_ptr<Entity> wall3 = core->createEntity();
		std::shared_ptr<MeshRenderer> MR3 = wall3->addComponent<MeshRenderer>("diffuse.bmp", "1b1cube.obj", glm::vec3(1.0f, 1.0f, 1.0f));
		
		wall3->transform()->setScale(glm::vec3(1.0f, 20.0f, 30.0f));  //Right
		wall3->transform()->m_position = glm::vec3(14.0f, 10.0f, 0.0f);
		std::shared_ptr<BoxCollider> b3 = wall3->addComponent<BoxCollider>();

		std::shared_ptr<Entity> wall4 = core->createEntity();
		std::shared_ptr<MeshRenderer> MR4 = wall4->addComponent<MeshRenderer>("diffuse.bmp", "1b1cube.obj", glm::vec3(1.0f, 1.0f, 1.0f));
		
		wall4->transform()->setScale(glm::vec3(30.0f, 20.0f, 1.0f)); //Back
		wall4->transform()->m_position = glm::vec3(0.0f, 10.0f, -14.0f);
		std::shared_ptr<BoxCollider> b4 = wall4->addComponent<BoxCollider>();

		std::shared_ptr<Entity> wall5 = core->createEntity();
		std::shared_ptr<MeshRenderer> MR5 = wall5->addComponent<MeshRenderer>("diffuse.bmp", "1b1cube.obj", glm::vec3(1.0f, 1.0f, 1.0f));		
		wall5->transform()->setScale(glm::vec3(30.0f, 1.0f, 30.0f)); //Roof
		wall5->transform()->m_position = glm::vec3(0.0f, 19.0f, 0.0f);
		std::shared_ptr<BoxCollider> b5 = wall5->addComponent<BoxCollider>();

		
	}

	void buildUI()
	{
		std::shared_ptr<Entity> e = getEntity();
		std::shared_ptr<Texture> t = getCore()->load<Texture>("Button.png");
		std::shared_ptr<ButtonUI> b = e->addComponent<ButtonUI>(t, 1);
		b->setSize(300, 150);
		float xpos = (getCore()->getContext()->getSize().x / 2.0f) - b->getSize().x / 2.0f;
		b->setPosition(xpos, 0.0f);
		button = b;
	}

	void onInitialise()
	{
		camera = getCore()->getDefaultCamera();
		camera.lock()->getEntity()->addComponent<AudioReceiver>();		
		camera.lock()->getEntity()->addComponent<MeshRenderer>("statue_diffuse.png", "1b1cube.obj", glm::vec3(1.0f, 1.0f, 1.0f));
		std::shared_ptr<BoxCollider> col = camera.lock()->getEntity()->addComponent<BoxCollider>();	
		col->m_trigger = true;

		camera.lock()->transform()->m_position += glm::vec3(0.0f, 5.0f, 5.0f);		

		std::shared_ptr<InputManager> IM = getCore()->getInput();
		mouse = IM->addDevice<Mouse>();
		keyboard = IM->addDevice<Keyboard>();		

		mouse.lock()->hideCursor(true);
		mouse.lock()->lockCursor(true);
		buildScene();
		buildUI();
	}

	void spawnSomeBalls()
	{
		//Create bouncy balls to see
		for (int i = 0; i < 5; i++)
		{
			std::shared_ptr<Entity> ball = getCore()->createEntity();
			std::shared_ptr<MeshRenderer> MR3 = ball->addComponent<MeshRenderer>("Image1.bmp", "1b1sphere.obj", glm::vec3(1.0f, 1.0f, 1.0f));
			ball->transform()->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
			ball->transform()->setPosition(glm::vec3(5.0f + (i * 0.2f), 2.0f + (i * 1.2f), 5.0f - i * 0.8f));
			std::shared_ptr<SphereCollider> sc = ball->addComponent<SphereCollider>();
			std::shared_ptr<AdvPhysicsObject> phys = ball->addComponent<AdvPhysicsObject>(1.0f, 0.85f);
			ball->addComponent<Ball>();
			balls.push_back(ball);
		}
	}

	void deleteBalls()
	{
		for (int i = 0; i < balls.size(); i++)
		{
			balls[i].lock()->destroy();
		}
		balls.clear();
	}

	void checkUserInput()
	{
		if (keyboard.lock()->GetKeyDown(SDLK_e))
		{
			spawnSomeBalls();
		}
		else if (keyboard.lock()->GetKeyDown(SDLK_q))
		{
			deleteBalls();
		}
		else if (keyboard.lock()->GetKeyDown(SDLK_ESCAPE))
		{
			getCore()->quit();
		}
		else if (keyboard.lock()->GetKeyDown(SDLK_SPACE))
		{
			mouse.lock()->hideCursor(false);
			mouse.lock()->lockCursor(false);
			lockedCursor = false;
		}
		else if (keyboard.lock()->GetKeyDown(SDLK_r))
		{
			mouse.lock()->hideCursor(true);
			mouse.lock()->lockCursor(true);
			lockedCursor = true;
		}
		else if (keyboard.lock()->GetKeyDown(SDLK_t))
		{
			throw Exception("Faulty Entity! Run! There is nothing that can save us!");
		}


		if (button.lock()->isButtonDown())
		{
			Console::output(Console::Message, "The Button you just pressed", "You can click a button, good for you.", true);
		}
	}

	void updateCamera()
	{
		float dTime = getCore()->getDeltaTime();
		std::shared_ptr<Transform> t = camera.lock()->transform();

		if (keyboard.lock()->GetKeyIsDown(SDLK_w))
		{
			t->m_position += (t->getFwd() * moveSpeed * dTime);
		}
		if (keyboard.lock()->GetKeyIsDown(SDLK_s))
		{
			t->m_position += (t->getFwd() * -moveSpeed * dTime);
		}
		if (keyboard.lock()->GetKeyIsDown(SDLK_d))
		{
			t->m_position += (t->getRight() * -moveSpeed * dTime);
		}
		if (keyboard.lock()->GetKeyIsDown(SDLK_a))
		{
			t->m_position += (t->getRight() * moveSpeed * dTime);
		}

		if (lockedCursor)
		{
			t->rotate(glm::vec3(0.0f, 1.0f, 0.0f), mouse.lock()->getDeltaPos().x * rotSpeed * dTime);
			t->rotate(glm::vec3(1.0f, 0.0f, 0.0f), -mouse.lock()->getDeltaPos().y * rotSpeed * dTime);
			if (t->m_eulerAngles.x > 89.0f) { t->m_eulerAngles.x = 89.0f; }
			else if (t->m_eulerAngles.x < -89.0f) { t->m_eulerAngles.x = -89.0f; }
			if (t->m_eulerAngles.y > 360.0f) { t->m_eulerAngles.y = 0.0f; }
			else if (t->m_eulerAngles.y < -360.0f) { t->m_eulerAngles.y = 0.0f; }
		}
	}

	void onTick()
	{
		updateCamera();
		checkUserInput();
	}
};




int main()
{
	std::shared_ptr<Core> core = Core::initialise(Core::Debug, glm::vec2(900, 900));

	//Create a demo object
	std::shared_ptr<Entity> demo = core->createEntity();
	demo->addComponent<Demo>();
	
	core->start();	
	return 0;	
}