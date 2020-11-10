#include "Core.h"
#include "Entity.h"
#include <iostream>

#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

namespace Engine
{
	std::shared_ptr<Core> Core::initialise()
	{		
		std::shared_ptr<Core> rtn = std::make_shared<Core>();
		rtn->initialiseSDL();	
		rtn->initialiseAL();
		rtn->m_input = new Input();
		rtn->m_scene = rtn->createScene();
		rtn->m_scene->createStartScene();
		rtn->LoadResources();

		std::cout << "Initialised successfully" << std::endl;
		return rtn;
	}

	std::shared_ptr<Entity> Core::createEntity()
	{
		std::shared_ptr<Entity> rtn = std::make_shared<Entity>();
		m_entities.push_back(rtn);
		std::cout << "I made an entity" << std::endl;
		return rtn;
	}

	void Core::loop()
	{
		// Re-initialise per-frame variables
		float time = SDL_GetTicks();
		float dTime = (time - t1) / 1000.0f;
		t1 = time;
		SDL_Event event = { 0 };

		SDL_GetMouseState(&mouseX, &mouseY);
		m_input->m_xOffset = mouseX - lastX;
		m_input->m_yOffset = mouseY - lastY;

		//Re-establish window-size to allow stretching and re-sizing
		SDL_GetWindowSize(m_window, &width, &height);

		for (size_t ei = 0; ei < m_entities.size(); ei++)
		{
			m_entities.at(ei)->tick(); //"Update"
		}

		glViewport(0, 0, width, height);

		//Set the clear-colour for the screen and clear it
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//Set the active texture buffer
		glActiveTexture(GL_TEXTURE0 + 1);
		m_scene->update(dTime);
		SDL_GL_SwapWindow(m_window);

		if (m_input->m_restart)
		{
			restart = true;
		}
		quit = m_input->takeInput(event); //Handles the input, and returns a 'quit' value to see if the program should end

		float targetTime = 1.0f / 60.f;
		if (targetTime > dTime) //The FPS cap
		{
			SDL_Delay((targetTime - dTime) * 1000.0f);
		}

		if (dTime > 100.0f) { dTime = 100.0f; }
		lastX = mouseX;
		lastY = mouseY;

		if (quit)
		{
			SDL_DestroyWindow(m_window); // DESTROY THAT WINDOW. STRIKE IT DOWN. DEWIT.
			SDL_Quit();
#if EMSCRIPTEN
			emscripten_cancel_main_loop();
#endif

			alcMakeContextCurrent(NULL);
			alcDestroyContext(m_context);
			alcCloseDevice(m_device);
		}
	}

	void Core::start()
	{
		std::cout << "I started to start" << std::endl;
		quit = false;			
		restart = false;
		halfX = WINDOW_WIDTH / 2;
		halfY = WINDOW_HEIGHT / 2;
		lastX = halfX;
		lastY = halfY;		 
		t1 = SDL_GetTicks(); //
		

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		SDL_ShowCursor(true);
		#ifdef EMSCRIPTEN
		emscripten_set_main_loop_arg([](void* _core) { ((Core*)_core)->loop(); }, this, 0, 1);
		#else
		while (!quit)
		{
			Core::loop();
		}
		#endif

	}

	void Core::initialiseSDL() // Starts the window and initialises a lot of the SDL/GLEW stuff
	{
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			throw std::exception();
		}

		m_window = SDL_CreateWindow("Delveworks",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

		if (!SDL_GL_CreateContext(m_window))
		{
			throw std::exception(); //TODO: Make better exception class
		}

		if (glewInit() != GLEW_OK)
		{
			throw std::exception();
		}
	}

	Scene* Core::createScene()
	{
		m_scene = new Scene();
		m_scene->m_core = this;
		return m_scene;
	}

	void Core::initialiseAL()
	{
		m_device = alcOpenDevice(NULL);
		if (!m_device)
		{
			throw std::exception();
		}

		m_context = alcCreateContext(m_device, NULL);
		if (!m_context)
		{
			alcCloseDevice(m_device);
			throw std::exception();
		}

		if (!alcMakeContextCurrent(m_context))
		{
			alcDestroyContext(m_context);
			alcCloseDevice(m_device);
			throw std::exception();
		}
	}

	void Core::LoadResources()
	{
			
		std::cout << "Successfully loaded shaders" << std::endl;
	}
}
