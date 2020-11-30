#include "Core.h"
#include "Entity.h"
#include <iostream>
#include "Component.h"
#include "Resource.h"
#include "Camera.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Light.h"

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
		rtn->initialiseShaders();

		rtn->m_self = rtn;
		rtn->createScreenQuad();
		rtn->createShadowMap();
		rtn->createRenderTexture();
		srand(time(NULL));

		//Create some necessary entities (TODO: Will be improved later)
		std::shared_ptr<Entity> e = rtn->createEntity();
		e->transform()->m_position = glm::vec3(0.0f, 8.0f, -8.0f);
		e->transform()->m_eulerAngles = glm::vec3(0.0f, -90.0f, -0.0f);
		rtn->m_camera = e->addComponent<Camera>();

		std::shared_ptr<Entity> sun = rtn->createEntity();
		rtn->m_sun = sun->addComponent<DirLight>();
		rtn->m_sun->setValues(glm::vec3(0.5f, 0.5f, 0.5f), 0.4f, glm::vec3(0.05f, 0.05f, 0.05f));
		rtn->m_sun->transform()->m_position = glm::vec3(0.0f, 20.0f, 0.0f);
		rtn->m_sun->transform()->m_eulerAngles = glm::vec3(90.0f, 0.0f, 0.0f);

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
		
		for (size_t ei = 0; ei < m_entities.size(); ei++)
		{
			m_entities.at(ei)->tick(); //"Update"
		}

		for (size_t ei = 0; ei < m_entities.size(); ei++)
		{
			m_entities.at(ei)->afterTick(); //A second tick for after-tick events
		}
				
		glUseProgram(m_shadowSh->getId());
		updateShadowMapShader();
		glBindFramebuffer(GL_FRAMEBUFFER, m_RTs[0]->fBufID);		
		glClear(GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, m_RTs[0]->resolutionX, m_RTs[0]->resolutionY);
		drawShadowScene();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_RTs[0]->m_textureId);
		glUseProgram(0);

		updateShader();
		glBindFramebuffer(GL_FRAMEBUFFER, m_RTs[1]->fBufID);
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, m_RTs[1]->resolutionX, m_RTs[1]->resolutionY);
		drawScene();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_RTs[1]->m_textureId);
			
		m_sqShader->draw(m_screenQuad);
		//draw a quad with any render textures on it (1 by default)

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

	void Core::drawShadowScene()
	{
		for (size_t ei = 0; ei < m_entities.size(); ei++)
		{
			std::shared_ptr<MeshRenderer> MR = m_entities.at(ei)->getComponent<MeshRenderer>();
			if (MR)
			{				
				m_shadowSh->setUniform("in_Model", MR->transform()->getModel()); // Translate the model matrix by camera position and stuff
				m_shadowSh->draw(MR->m_vAO);
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
		model = glm::lookAt(m_camera->transform()->getPosition(), m_camera->transform()->getPosition() + m_camera->transform()->getFwd(), m_camera->transform()->getUp());
		
		m_lightingSh->setUniform("in_View", model); // Establish the view matrix		
		m_lightingSh->setUniform("in_Emissive", glm::vec3(0.0f, 0.0f, 0.0f));		
		m_lightingSh->setUniform("in_CamPos", m_camera->transform()->m_position);
		m_lightingSh->setUniform("in_shadowMap", m_RTs[0]);

		//Shader for the screen quad (For render textures)
		m_sqShader->setUniform("in_Projection", glm::ortho(-1, 1, -1, 1));
		m_sqShader->setUniform("in_Texture", 0);
	}

	void Core::updateShadowMapShader() //TODO: support multiple directional lights (Pass in arrays instead)
	{
		//Shadow Shaders
		m_shadowSh->setUniform("in_Projection", glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 1.0f, 50.0f)); //Set the projection uniform
		glm::mat4 model(1.0f);
		model = glm::lookAt(m_sun->transform()->getPosition(), m_sun->transform()->getPosition() + m_sun->transform()->getFwd(), m_sun->transform()->getUp()); 
		m_shadowSh->setUniform("in_View", model); // Establish the view matrix		
	}

	std::shared_ptr<RenderTexture> Core::createRenderTexture()
	{
		std::shared_ptr<RenderTexture> RT = std::make_shared<RenderTexture>();
		RT->Initialise();
		m_RTs.push_back(RT);
		return RT;
	}

	std::shared_ptr<ShadowMap> Core::createShadowMap()
	{
		std::shared_ptr<ShadowMap> SM = std::make_shared<ShadowMap>();
		SM->Initialise();
		m_RTs.push_back(SM);
		return SM;
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

	void Core::initialiseShaders()
	{
		m_sqShader = m_rManager->load<Shader>("UI");
		m_lightingSh = m_rManager->load<Shader>("light");
		m_shadowSh = m_rManager->load<Shader>("dirShadow");
	}
}
