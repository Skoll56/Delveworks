#include "Core.h"
#include "Entity.h"
#include <iostream>
#include "Component.h"
#include "Resource.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Light.h"
#include "RenderTexture.h"
#include "Exception.h"


#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

namespace Engine
{
	std::shared_ptr<Core> Core::initialise()
	{
		std::shared_ptr<Core> rtn = std::make_shared<Core>();
		rtn->m_rManager = std::make_shared<ResourceManager>();
		rtn->initialiseSDL();
		rtn->initialiseAL();
		rtn->m_inputManager = std::make_shared<InputManager>();
		rtn->m_inputManager->m_window = rtn->m_window;
		rtn->m_inputManager->m_self = rtn->m_inputManager;
		rtn->m_inputManager->m_core = rtn;//
		//

		rtn->initialiseShaders();

		rtn->m_self = rtn;//
		//
		rtn->createScreenQuad();

		rtn->createRenderTexture();
		srand(time(NULL));

		//Create some necessary entities (TODO: Will be improved later)
		std::shared_ptr<Entity> e = rtn->createEntity();
		e->transform()->m_position = glm::vec3(0.0f, 4.0f, -12.0f);
		e->transform()->m_eulerAngles = glm::vec3(0.0f, -90.0f, -0.0f);
		rtn->m_camera = e->addComponent<Camera>();

		Console::message("Initialised successfully");
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
		Console::message("I made an entity");
		return rtn;
	}

	void Core::loop()
	{
		// Re-initialise per-frame variables
		float time = SDL_GetTicks();
		dTime = (time - t1) / 1000.0f;
		t1 = time;		
		
		//Set the clear-colour for the screen and clear it
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		quit = m_inputManager->update(); //Handles the input, and returns a 'quit' value to see if the program should end		
		//Re-establish window-size to allow stretching and re-sizing
		SDL_GetWindowSize(m_window, &width, &height);		
		
		getCurrentCamera()->update(dTime);
		updateEntities();
		updateLighting();				
		drawShadowmaps(); /* <-- !GRAPHICS UNIT! */
		renderScreen();

		float targetTime = 1.0f / 60.f;
		if (targetTime > dTime) //The FPS cap
		{
			SDL_Delay((targetTime - dTime) * 1000.0f);
		}
		
		
		if (quit) 
		{
			m_inputManager->closeInputDevices();
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
			try
			{
				std::shared_ptr<MeshRenderer> MR = m_entities.at(ei)->getComponent<MeshRenderer>();
				if (MR)
				{
					MR->draw();
				}
			}
			catch (Exception &e)
			{
				Console::output(Console::Error, "Draw Scene", e.message());
			}
		}
	}

	/* !This has been CREATED as part of the GRAPHICS UNIT! */
	void Core::drawShadowScene()
	{
		for (size_t ei = 0; ei < m_entities.size(); ei++)
		{
			try
			{
				std::shared_ptr<MeshRenderer> MR = m_entities.at(ei)->getComponent<MeshRenderer>();
				if (MR)
				{
					m_shadowSh->setUniform("in_Model", MR->transform()->getModel()); // Translate the model matrix by camera position and stuff
					m_shadowSh->draw(MR->m_vAO);
				}
			}
			catch(Exception &e)
			{
				Console::output(Console::Error, "Draw Shadowmap", e.message());
			}
		}
	}

	/* !This has been CREATED as part of the GRAPHICS UNIT! */
	void Core::drawPointShadowScene()
	{
		for (size_t ei = 0; ei < m_entities.size(); ei++)
		{ 
			try
			{
				std::shared_ptr<MeshRenderer> MR = m_entities.at(ei)->getComponent<MeshRenderer>();
				if (MR)
				{
					m_pointShadowSh->setUniform("in_Model", MR->transform()->getModel()); // Translate the model matrix by camera position and stuff
					m_pointShadowSh->draw(MR->m_vAO);
				}
			}
			catch (Exception &e)
			{
				Console::output(Console::Error, "Draw Shadowcube", e.message());
			}
		}
	}

	void Core::start()
	{
		Console::message("I started to start");
		quit = false;
		restart = false;
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
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) < 0)
		{
			Console::output(Console::FatalError, "Core", "Failed to initialise SDL");
			throw Exception("Failed to Initialise SDL"); //
		}

		
		//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		m_window = SDL_CreateWindow("Delveworks",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			width, height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);


		if (!SDL_GL_CreateContext(m_window))
		{
			throw Exception("Failed to create SDL Window");
		}
		
		if (glewInit() != GLEW_OK)//
		{
			throw Exception("Failed to initialise GLEW");
		}
	}

	/* !This has been MODIFIED as part of the GRAPHICS UNIT! */
	void Core::updateShader()
	{
		try
		{
			//Lighting Shaders
			m_lightingSh->setUniform("in_Projection", glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f)); //Set the projection uniform
			m_lightingSh->setUniform("in_View", getCurrentCamera()->getView()); // Establish the view matrix		
			m_lightingSh->setUniform("in_Emissive", glm::vec3(0.0f, 0.0f, 0.0f));
			m_lightingSh->setUniform("in_CamPos", getCurrentCamera()->transform()->m_position);

			//Shader for the screen quad (For render textures)
			m_sqShader->setUniform("in_Projection", glm::ortho(-1, 1, -1, 1));
			//Change to Shadowmap to view depth buffer
			m_sqShader->setUniform("in_Texture", m_RT);
			/*m_sqShader->setUniform("in_Texture", m_dirLights[0]->getShadowMap());
			m_sqShader->setUniform("in_nearPlane", 0.01f);
			m_sqShader->setUniform("in_farPlane", m_pointLights[0]->getRadius());*/
		}
		catch (Exception &e)
		{
			Console::output(Console::Error, "Update Shaders", e.message());
		}
	}

	/* !This has been CREATED as part of the GRAPHICS UNIT! */
	void Core::drawShadowmaps() 
	{
		for (int i = 0; i < m_dirLights.size(); i++)
		{
			try
			{
				std::shared_ptr<ShadowMap> shadowmap = m_dirLights[i].lock()->getShadowMap();
				glUseProgram(m_shadowSh->getId());
				m_shadowSh->setUniform("in_LightSpaceMatrix", m_dirLights[i].lock()->getShadowMap()->getLightSpaceMatrix());
				glBindFramebuffer(GL_FRAMEBUFFER, shadowmap->fBufID);
				glClear(GL_DEPTH_BUFFER_BIT);
				glViewport(0, 0, shadowmap->resolutionX, shadowmap->resolutionY);
				drawShadowScene();
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				glUseProgram(0);
			}
			catch (Exception &e)
			{
				Console::output(Console::Error, "Draw Shadowmaps", e.message());
			}
		}

		for (int i = 0; i < m_pointLights.size(); i++)
		{
			try
			{
				std::shared_ptr<ShadowCube> SC = m_pointLights[i].lock()->getShadowCube();
				glUseProgram(m_shadowSh->getId());
				glBindFramebuffer(GL_FRAMEBUFFER, SC->fBufID);


				m_pointShadowSh->setUniform("in_lightPos", m_pointLights[i].lock()->transform()->getPosition());
				m_pointShadowSh->setUniform("in_farPlane", m_pointLights[i].lock()->getRadius());

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
			catch (Exception &e)
			{
				Console::output(Console::Error, "Pointlight Update", e.message());
			}
		}

		for (int i = 0; i < m_spotLights.size(); i++)
		{
			try
			{
				std::shared_ptr<ShadowMap> shadowmap = m_spotLights[i].lock()->getShadowMap();
				glUseProgram(m_shadowSh->getId());
				m_shadowSh->setUniform("in_LightSpaceMatrix", m_spotLights[i].lock()->getShadowMap()->getLightSpaceMatrix());
				glBindFramebuffer(GL_FRAMEBUFFER, shadowmap->fBufID);
				glClear(GL_DEPTH_BUFFER_BIT);
				glViewport(0, 0, shadowmap->resolutionX, shadowmap->resolutionY);
				drawShadowScene();
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				glUseProgram(0);
			}
			catch (Exception &e)
			{
				Console::output(Console::Error, "Spotlight Update", e.message());
			}
		}		
	}

	/* !This has been CREATED as part of the GRAPHICS UNIT! */
	std::shared_ptr<RenderTexture> Core::createRenderTexture()
	{
		std::shared_ptr<RenderTexture> RT = std::make_shared<RenderTexture>();
		RT->Initialise(width, height);	
		m_RT = RT;
		return RT;
	}
	
	void Core::createScreenQuad()
	{
		m_screenQuad = std::make_shared<VertexArray>();
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
			throw Exception("Failed to create ALC Device");
		}

		m_context = alcCreateContext(m_device, NULL);
		if (!m_context)
		{
			alcCloseDevice(m_device);
			throw Exception("Failed to create SDL Window");
		}

		if (!alcMakeContextCurrent(m_context))
		{
			alcDestroyContext(m_context);
			alcCloseDevice(m_device);
			throw Exception("Failed to create ALC Context");
		}
	}

	void Core::resizeWindow(int _x, int _y)
	{
		m_RT->Initialise(_x, _y);
	}

	std::shared_ptr<Camera> Core::getCurrentCamera()
	{
		return m_camera.lock();
	}

	void Core::initialiseShaders()
	{
		m_sqShader = m_rManager->load<Shader>("UI");
		m_lightingSh = m_rManager->load<Shader>("light");
		m_shadowSh = m_rManager->load<Shader>("dirShadow");
		m_pointShadowSh = m_rManager->load<Shader>("pointShadow");
	}

	void Core::updateLighting()
	{
		for (std::vector<std::weak_ptr<DirLight>>::iterator it = m_dirLights.begin(); it != m_dirLights.end();)
		{
			try
			{
				if ((*it).lock())
				{
					(*it).lock()->update(std::distance(m_dirLights.begin(), it));
					it++;
				}
				else //This object got deleted
				{
					it = m_dirLights.erase(it);
				}
			}
			catch (Exception &e)
			{
				Console::output(Console::Error, "DirLight Update", e.message());
			}
		}

		for (std::vector<std::weak_ptr<PointLight>>::iterator it = m_pointLights.begin(); it != m_pointLights.end();)
		{
			try
			{
				if ((*it).lock())
				{
					(*it).lock()->update(std::distance(m_pointLights.begin(), it));
					it++;
				}
				else //This object got deleted
				{
					it = m_pointLights.erase(it);
				}
			}
			catch (Exception &e)
			{
				Console::output(Console::Error, "pointLight Update", e.message());
			}
		}

		for (std::vector<std::weak_ptr<SpotLight>>::iterator it = m_spotLights.begin(); it != m_spotLights.end();)
		{
			try
			{
				if ((*it).lock())
				{
					(*it).lock()->update(std::distance(m_spotLights.begin(), it));
					it++;
				}
				else //This object got deleted
				{
					it = m_spotLights.erase(it);
				}
			}
			catch (Exception &e)
			{
				Console::output(Console::Error, "spotLight Update", e.message());
			}
		}
	}

	void Core::updateEntities()
	{
		for (std::vector<std::shared_ptr<Entity>>::iterator it = m_entities.begin(); it != m_entities.end(); it++)
		{
			try
			{
				(*it)->tick(); //"Update"
			}
			catch (Exception &e)
			{
				Console::output(Console::Error, (*it)->getTag(), e.message());
			}
		}
		for (std::vector<std::shared_ptr<Entity>>::iterator it = m_entities.begin(); it != m_entities.end();)
		{
			try
			{
				(*it)->afterTick(); //A second tick for after-tick events
				if ((*it)->m_delete)
				{
					it = m_entities.erase(it);
				}
				else
				{
					it++;
				}
			}
			catch (Exception &e)
			{
				Console::output(Console::Error, (*it)->getTag(), e.message());
				it++;
			}
		}
	}

	void Core::renderScreen()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RT->fBufID);
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, width, height);
		updateShader();
		drawScene();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		m_sqShader->draw(m_screenQuad); //draw a quad with any render textures on it (1 by default)		
		SDL_GL_SwapWindow(m_window);
	}

}
