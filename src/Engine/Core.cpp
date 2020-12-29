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
#include "RenderTexture.h"

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
		rtn->m_input = std::make_shared<Input>();
		rtn->initialiseShaders();

		rtn->m_self = rtn;
		rtn->createScreenQuad();		
		rtn->createRenderTexture();
		srand(time(NULL));

		//Create some necessary entities (TODO: Will be improved later)
		std::shared_ptr<Entity> e = rtn->createEntity();
		e->transform()->m_position = glm::vec3(0.0f, 4.0f, -12.0f);
		e->transform()->m_eulerAngles = glm::vec3(0.0f, -90.0f, -0.0f);
		rtn->m_camera = e->addComponent<Camera>();

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
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		SDL_GetMouseState(&mouseX, &mouseY);
		m_input->m_xOffset = mouseX - WINDOW_WIDTH /2;
		m_input->m_yOffset = mouseY - WINDOW_HEIGHT/2;

		SDL_WarpMouseInWindow(m_window, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

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

		for (int di = 0; di < m_dirLights.size(); di++)
		{
			m_dirLights.at(di)->update(di); 
		}

		for (int si = 0; si < m_spotLights.size(); si++)
		{
			m_spotLights.at(si)->update(si); 
		}

		for (int pi = 0; pi < m_pointLights.size(); pi++)
		{
			m_pointLights.at(pi)->update(pi);
		}
				
		drawShadowmaps(); /* <-- !GRAPHICS UNIT! */

		updateShader();
		glBindFramebuffer(GL_FRAMEBUFFER, m_RT->fBufID);
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, m_RT->resolutionX, m_RT->resolutionY);
		drawScene();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
			
		m_sqShader->draw(m_screenQuad);
		//draw a quad with any render textures on it (1 by default)

		SDL_GL_SwapWindow(m_window);

		quit = m_input->takeInput(event); //Handles the input, and returns a 'quit' value to see if the program should end
		

		float targetTime = 1.0f / 60.f;
		if (targetTime > dTime) //The FPS cap
		{
			SDL_Delay((targetTime - dTime) * 1000.0f);
		}
		
		
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

	/* !This has been MODIFIED as part of the GRAPHICS UNIT! */
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

	/* !This has been CREATED as part of the GRAPHICS UNIT! */
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

	/* !This has been CREATED as part of the GRAPHICS UNIT! */
	void Core::drawPointShadowScene()
	{
		for (size_t ei = 0; ei < m_entities.size(); ei++)
		{
			std::shared_ptr<MeshRenderer> MR = m_entities.at(ei)->getComponent<MeshRenderer>();
			if (MR)
			{
				m_pointShadowSh->setUniform("in_Model", MR->transform()->getModel()); // Translate the model matrix by camera position and stuff
				m_pointShadowSh->draw(MR->m_vAO);
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
		SDL_ShowCursor(false);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

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

	/* !This has been MODIFIED as part of the GRAPHICS UNIT! */
	void Core::updateShader()
	{
		//Lighting Shaders
		m_lightingSh->setUniform("in_Projection", glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f)); //Set the projection uniform
		glm::mat4 view(1.0f);
		view = glm::lookAt(m_camera->transform()->getPosition(), m_camera->transform()->getPosition() + m_camera->transform()->getFwd(), m_camera->transform()->getUp());		

		m_lightingSh->setUniform("in_View", view); // Establish the view matrix		
		m_lightingSh->setUniform("in_Emissive", glm::vec3(0.0f, 0.0f, 0.0f));
		m_lightingSh->setUniform("in_CamPos", m_camera->transform()->m_position);

		//Shader for the screen quad (For render textures)
		m_sqShader->setUniform("in_Projection", glm::ortho(-1, 1, -1, 1));
		//Change to Shadowmap to view depth buffer
		m_sqShader->setUniform("in_Texture", m_RT);
		/*m_sqShader->setUniform("in_Texture", m_dirLights[0]->getShadowMap());
		m_sqShader->setUniform("in_nearPlane", 0.01f);
		m_sqShader->setUniform("in_farPlane", m_pointLights[0]->getRadius());*/

	}

	/* !This has been CREATED as part of the GRAPHICS UNIT! */
	void Core::drawShadowmaps() 
	{
		for (int i = 0; i < m_dirLights.size(); i++)
		{
			std::shared_ptr<ShadowMap> shadowmap = m_dirLights[i]->getShadowMap();
			glUseProgram(m_shadowSh->getId());
			m_shadowSh->setUniform("in_LightSpaceMatrix", m_dirLights[i]->getShadowMap()->getLightSpaceMatrix());
			glBindFramebuffer(GL_FRAMEBUFFER, shadowmap->fBufID);
			glClear(GL_DEPTH_BUFFER_BIT);
			glViewport(0, 0, shadowmap->resolutionX, shadowmap->resolutionY);
			drawShadowScene();
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glUseProgram(0);
		}

		for (int i = 0; i < m_pointLights.size(); i++)
		{
			std::shared_ptr<ShadowCube> SC = m_pointLights[i]->getShadowCube();
			glUseProgram(m_shadowSh->getId());
			glBindFramebuffer(GL_FRAMEBUFFER, SC->fBufID);


			m_pointShadowSh->setUniform("in_lightPos", m_pointLights[i]->transform()->getPosition());
			m_pointShadowSh->setUniform("in_farPlane", m_pointLights[i]->getRadius());

			for (int l = 0; l < 6; l++)
			{
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X + l, SC->m_textureId, 0);
				glClear(GL_DEPTH_BUFFER_BIT);
				m_pointShadowSh->setUniform("in_LightSpaceMatrix", SC->getMatrix(l));
				glViewport(0, 0, SC->resolutionX, SC->resolutionY);
				drawPointShadowScene();
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glUseProgram(0);
		}

		for (int i = 0; i < m_spotLights.size(); i++)
		{
			std::shared_ptr<ShadowMap> shadowmap = m_spotLights[i]->getShadowMap();
			glUseProgram(m_shadowSh->getId());
			m_shadowSh->setUniform("in_LightSpaceMatrix", m_spotLights[i]->getShadowMap()->getLightSpaceMatrix());
			glBindFramebuffer(GL_FRAMEBUFFER, shadowmap->fBufID);
			glClear(GL_DEPTH_BUFFER_BIT);
			glViewport(0, 0, shadowmap->resolutionX, shadowmap->resolutionY);
			drawShadowScene();
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glUseProgram(0);
		}		

		
	}

	/* !This has been CREATED as part of the GRAPHICS UNIT! */
	std::shared_ptr<RenderTexture> Core::createRenderTexture()
	{
		std::shared_ptr<RenderTexture> RT = std::make_shared<RenderTexture>();
		RT->Initialise();	
		m_RT = RT;
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
		m_pointShadowSh = m_rManager->load<Shader>("pointShadow");
	}
}
