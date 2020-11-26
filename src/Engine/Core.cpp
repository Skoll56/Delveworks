#include "Core.h"
#include "Entity.h"
#include <iostream>
#include "Component.h"
#include "Resource.h"
#include "Camera.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 1024

namespace Engine
{
	std::shared_ptr<Core> Core::initialise()
	{
		std::shared_ptr<Core> rtn = std::make_shared<Core>();
		rtn->m_rManager = std::make_shared<ResourceManager>();
		rtn->initialiseSDL();
		rtn->initialiseAL();
		rtn->m_input = new Input();
		rtn->m_lightingSh = rtn->m_rManager->load<Shader>("light");
		rtn->m_camera = std::make_shared<Camera>(glm::vec3(0.0f, 5.0f, 0.0f));
		rtn->m_self = rtn;
		rtn->createScreenQuad();
		rtn->createRenderTexture();
		srand(time(NULL));

		std::cout << "Initialised successfully" << std::endl;
		return rtn;
	}

	std::shared_ptr<Entity> Core::createEntity()
	{
		std::shared_ptr<Entity> rtn = std::make_shared<Entity>();
		rtn->self = rtn;
		rtn->core = m_self;
		rtn->addComponent<Transform>();

		std::string tag = "";
		for (int i = 0; i < 15; i++)
		{
			tag += (rand() % 8000);
		}
		rtn->setTag(tag);

		m_entities.push_back(rtn);
		std::cout << "I made an entity" << std::endl;
		return rtn;
	}

	void Core::loop()
	{
		// Re-initialise per-frame variables
		float time = SDL_GetTicks();
		dTime = (time - t1) / 1000.0f;
		t1 = time;
		SDL_Event event = { 0 };
		
		//Set the clear-colour for the screen and clear it
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		SDL_GetMouseState(&mouseX, &mouseY);
		m_input->m_xOffset = mouseX - lastX;
		m_input->m_yOffset = mouseY - lastY;

		//Re-establish window-size to allow stretching and re-sizing
		SDL_GetWindowSize(m_window, &width, &height);
		m_camera->update(dTime, m_input);
		updateShader();
		for (size_t ei = 0; ei < m_entities.size(); ei++)
		{
			m_entities.at(ei)->tick(); //"Update"
		}

		for (size_t ei = 0; ei < m_entities.size(); ei++)
		{
			m_entities.at(ei)->afterTick(); //A second tick for after-tick events
		}

		for (size_t ri = 0; ri < m_RTs.size(); ri++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_RTs[ri]->fBufID);
			glClearColor(0.0, 0.0, 0.0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glViewport(0, 0, width, height);
			drawScene();
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_RTs[ri]->fBufTexID);
		}		
		m_sqShader->draw(m_screenQuad);
		//draw a quad with any render textures on it (1 by default)

		//Set the active texture buffer
		//glActiveTexture(GL_TEXTURE0 + 1);
		SDL_GL_SwapWindow(m_window);

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

	void Core::drawScene()
	{
		for (size_t ei = 0; ei < m_entities.size(); ei++)
		{
			std::shared_ptr<MeshRenderer> MR = m_entities.at(ei)->getComponent<MeshRenderer>();
			if (MR)
			{
				MR->draw();
			}
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
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		{
			throw std::exception();
		}

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

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

	void Core::updateShader()
	{
		//Lighting Shaders
		m_lightingSh->setUniform("in_Projection", glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f)); //Set the projection uniform
		glm::mat4 model(1.0f);
		model = glm::lookAt(m_camera->getPos(), m_camera->getPos() + m_camera->getFwd(), m_camera->getUp());
		
		m_lightingSh->setUniform("in_View", model); // Establish the view matrix		
		m_lightingSh->setUniform("in_Emissive", glm::vec3(0.0f, 0.0f, 0.0f));		
		m_lightingSh->setUniform("in_CamPos", m_camera->getPos());

		//Shader for the screen quad (For render textures)
		m_sqShader->setUniform("in_Projection", glm::ortho(-1, 1, -1, 1));
		m_sqShader->setUniform("in_Texture", 0);
		
		//m_quad_shader->setUniform("in_View", glm::mat4(1.0f));
	}

	std::shared_ptr<RenderTexture> Core::createRenderTexture()
	{
		std::shared_ptr<RenderTexture> RT = std::make_shared<RenderTexture>();
		RT->Initialise();
		m_RTs.push_back(RT);
		return RT;
	}

	void Core::createScreenQuad()
	{
		m_screenQuad = new VertexArray();
		m_screenQuad->setBuffer("in_Position", new VertexBuffer());
		m_screenQuad->setBuffer("in_TexCoord", new VertexBuffer());

		m_screenQuad->getTriPos()->add(glm::vec3(-1.0f, -1.0f, 0.0f));
		m_screenQuad->getTriPos()->add(glm::vec3(1.0f, -1.0f, 0.0f));
		m_screenQuad->getTriPos()->add(glm::vec3(1.0f, 1.0f, 0.0f));
		m_screenQuad->getTriPos()->add(glm::vec3(1.0f, 1.0f, 0.0f));
		m_screenQuad->getTriPos()->add(glm::vec3(-1.0f, 1.0f, 0.0f));
		m_screenQuad->getTriPos()->add(glm::vec3(-1.0f, -1.0f, 0.0f));

		m_screenQuad->getTriTex()->add(glm::vec2(0.0f, 0.0f));
		m_screenQuad->getTriTex()->add(glm::vec2(1.0f, 0.0f));
		m_screenQuad->getTriTex()->add(glm::vec2(1.0f, 1.0f));
		m_screenQuad->getTriTex()->add(glm::vec2(1.0f, 1.0f));
		m_screenQuad->getTriTex()->add(glm::vec2(0.0f, 1.0f));
		m_screenQuad->getTriTex()->add(glm::vec2(0.0f, 0.0f));
		m_sqShader = m_rManager->load<Shader>("UI");
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
}
