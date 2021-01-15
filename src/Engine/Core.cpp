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
//#include "Surface.h"


#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

namespace Engine
{
	std::shared_ptr<Core> Core::initialise(Core::RunMode _mode, glm::vec2 _gameWindowSize)
	{
		std::shared_ptr<Core> rtn = std::make_shared<Core>();
		rtn->m_rManager = std::make_shared<ResourceManager>();
		if (_mode == Core::Debug)
		{
			rtn->m_width = _gameWindowSize.x + 428;
			rtn->m_height = _gameWindowSize.y + 56;
		}
		else
		{
			rtn->m_width = _gameWindowSize.x;
			rtn->m_height = _gameWindowSize.y;
		}

		rtn->initialiseSDL();
		rtn->initialiseAL();
		rtn->m_inputManager = std::make_shared<InputManager>();
		rtn->m_inputManager->m_window = rtn->m_window;
		rtn->m_inputManager->m_self = rtn->m_inputManager;
		rtn->m_inputManager->m_core = rtn;
		rtn->initialiseShaders();
		rtn->m_self = rtn;		
		srand(time(NULL));

		//Create some necessary entities 
		std::shared_ptr<Entity> defaultEntity = rtn->createEntity();
		std::shared_ptr<Camera> cam = defaultEntity->addComponent<Camera>();
		rtn->setDefaultCamera(cam);
		
		rtn->m_gameContext = std::make_shared<Context>();
		rtn->m_gameContext->m_size = _gameWindowSize;
		if (_mode == Core::Debug)
		{
			rtn->m_gameContext->m_position = glm::vec2(400, 28);
			rtn->buildEngineUI();
			
		}

		std::shared_ptr<DisplayUI> s = defaultEntity->addComponent<DisplayUI>(cam, 0);
		s->setSize(s->m_context.lock()->m_size);
		

		Console::output("Initialised successfully");
		//rtn->start();
		return rtn;
	}

	std::shared_ptr<Entity> Core::createEntity()
	{
		std::shared_ptr<Entity> rtn = std::make_shared<Entity>();
		rtn->m_self = rtn;
		rtn->m_core = m_self;
		rtn->addComponent<Transform>();

		std::string tag = "";
		for (int i = 0; i < 15; i++)
		{
			tag += (rand() % 8000);
		}
		rtn->setTag(tag);

		m_entities.push_back(rtn);		
		return rtn;
	}

	void Core::loop()
	{
		// Re-initialise per-frame variables
		float time = SDL_GetTicks();
		m_dTime = (time - m_t1) / 1000.0f;
		if (m_dTime > 0.1f) { m_dTime = 0.1f; } //Minimum dTime added to prevent falling through the floor and physics objects breaking while the Window is held
		m_t1 = time;		
		
		//Set the clear-colour for the screen and clear it
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_quit = m_inputManager->update(); //Handles the input, and returns a 'quit' value to see if the program should end		
		//Re-establish window-size to allow stretching and re-sizing
		SDL_GetWindowSize(m_window, &m_width, &m_height);		
		
		
		updateEntities();
		updateLighting();		
		#ifndef EMSCRIPTEN
		drawShadowmaps(); /* <-- !GRAPHICS UNIT! */
		#endif
		renderScreen();

		float targetTime = 1.0f / 60.f;
		if (targetTime > m_dTime) //The FPS cap
		{
			SDL_Delay((targetTime - m_dTime) * 1000.0f);
		}
		
		
		if (m_quit) 
		{

			Console::printToFile();
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
		std::vector<std::shared_ptr<MeshRenderer>> drawLater;

		for (size_t ei = 0; ei < m_entities.size(); ei++)
		{
			try
			{
				if (m_entities[ei]->isActive())
				{
					std::shared_ptr<MeshRenderer> MR = m_entities.at(ei)->getComponent<MeshRenderer>();
					if (MR)
					{
						if (MR->getAlpha() < 1.0f)
						{
							drawLater.push_back(MR); //Transparent objects need to be drawn last, because of reasons and things.
						}
						else
						{
							MR->draw();
						}
					}
				}
			}
			catch (Exception &e)
			{
				Console::output(Console::Error, "Draw Scene", e.message());
			}
		}

		for (size_t ei = 0; ei < drawLater.size(); ei++)
		{
			try
			{				
				//glDepthMask(false);
				drawLater[ei]->draw();	
				//glDepthMask(true);
			}
			catch (Exception &e)
			{
				Console::output(Console::Error, "Draw Transparent Scene", e.message());
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
				if (m_entities[ei]->isActive())
				{
					std::shared_ptr<MeshRenderer> MR = m_entities.at(ei)->getComponent<MeshRenderer>();
					if (MR)
					{
						if (MR->getCastShadows())
						{
							m_shadowSh->setUniform("in_Model", MR->transform()->getModel()); // Translate the model matrix by camera position and stuff
							m_shadowSh->draw(MR->getMesh());
						}
					}
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
				if (m_entities[ei]->isActive())
				{
					std::shared_ptr<MeshRenderer> MR = m_entities.at(ei)->getComponent<MeshRenderer>();
					if (MR)
					{
						if (MR->getCastShadows())
						{
							m_pointShadowSh->setUniform("in_Model", MR->transform()->getModel()); // Translate the model matrix by camera position and stuff
							m_pointShadowSh->draw(MR->getMesh());
						}
					}
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
		m_quit = false;		
		m_t1 = SDL_GetTicks(); 
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);		
		
		#ifdef EMSCRIPTEN
		emscripten_set_main_loop_arg([](void* _core) { ((Core*)_core)->loop(); }, this, 0, 1);
		#else
		while (!m_quit)
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
			m_width, m_height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);


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
	void Core::updateLightingShader(std::shared_ptr<Camera> _cam, glm::vec2 _viewport)
	{
		try
		{
			//Lighting Shaders
			m_lightingSh->setUniform("in_Projection", glm::perspective(glm::radians(_cam->getFOV()), (float)_viewport.x / (float)_viewport.y, 0.1f, 100.0f)); //Set the projection uniform
			m_lightingSh->setUniform("in_View", _cam->getView()); // Establish the view matrix					
			m_lightingSh->setUniform("in_CamPos", _cam->transform()->m_position);

			int d = m_dirLights.size();
			if (d > m_maxDir) { d = m_maxDir; }

			int s = m_spotLights.size();
			if (s > m_maxSpot) { s = m_maxSpot; }

			int p = m_pointLights.size();
			if (p > m_maxPoint) { p = m_maxPoint; }

			m_lightingSh->setUniform("in_numDir", d);
			m_lightingSh->setUniform("in_numSpot", s);
			m_lightingSh->setUniform("in_numPoint", p);
		}
		catch (Exception &e)
		{
			Console::output(Console::Error, "Update Light Shaders", e.message());
		}
	}

	void Core::updateSurfaceShader(std::shared_ptr<Texture> _tex, float _alpha, glm::vec3 _color)
	{
		try
		{
			//Shader for the screen quad (For render textures)
			m_sqShader->setUniform("in_Projection", glm::ortho(-1, 1, -1, 1));
			//Change to Shadowmap to view depth buffer
			m_sqShader->setUniform("in_Texture", _tex);
			m_sqShader->setUniform("in_alpha", _alpha);
			m_sqShader->setUniform("in_col", _color);
			/*m_sqShader->setUniform("in_Texture", m_dirLights[0]->getShadowMap());
			m_sqShader->setUniform("in_nearPlane", 0.01f);
			m_sqShader->setUniform("in_farPlane", m_pointLights[0]->getRadius());*/
		}
		catch (Exception &e)
		{
			Console::output(Console::Error, "Update Surface Shaders", e.message());
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
				m_shadowSh->setUniform("in_LightSpaceMatrix", m_dirLights[i].lock()->getShadowMap()->getMatrix());
				glBindFramebuffer(GL_FRAMEBUFFER, shadowmap->getFrameBufID());
				glClear(GL_DEPTH_BUFFER_BIT);
				glViewport(0, 0, shadowmap->getResolution().x, shadowmap->getResolution().y);
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
				glBindFramebuffer(GL_FRAMEBUFFER, SC->getFrameBufID());


				m_pointShadowSh->setUniform("in_lightPos", m_pointLights[i].lock()->transform()->getPosition());
				m_pointShadowSh->setUniform("in_farPlane", m_pointLights[i].lock()->getRadius());

				for (int l = 0; l < 6; l++)
				{
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X + l, SC->get(), 0);
					glClear(GL_DEPTH_BUFFER_BIT);
					m_pointShadowSh->setUniform("in_LightSpaceMatrix", SC->getMatrix(l));
					glViewport(0, 0, SC->getResolution().x, SC->getResolution().y);
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
				m_shadowSh->setUniform("in_LightSpaceMatrix", m_spotLights[i].lock()->getShadowMap()->getMatrix());
				glBindFramebuffer(GL_FRAMEBUFFER, shadowmap->getFrameBufID());
				glClear(GL_DEPTH_BUFFER_BIT);
				glViewport(0, 0, shadowmap->getResolution().x, shadowmap->getResolution().y);
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

	void Core::buildEngineUI() //
	{		
		m_engineContext = std::make_shared<Context>();		
		for (int i = 0; i < 4; i++)
		{			
			std::shared_ptr<ImageUI> s = std::make_shared<ImageUI>();
			std::shared_ptr<Texture> t = m_rManager->load<Texture>("defaultUI.bmp");
			s->m_tex = t;
			s->createScreenQuad();
			s->m_layer = i;			
			s->m_context = m_engineContext;
			s->m_self = s;
			s->m_col = glm::vec3(0.35f, 0.0f, 0.35f);

			if (i == 0) //Left banner
			{
				s->setSize(400, m_height);
				s->setPosition(0, 0);
			}
			else if (i == 1) //Bottom banner
			{
				s->setSize(m_width, 28);
				s->setPosition(0, 0);
			}
			else if (i == 2) //Top banner
			{
				s->setSize(m_width, 28);
				s->setPosition(0, m_height - 28);
			}
			else if (i == 3) //Right banner
			{
				s->setSize(28, m_height);
				s->setPosition(m_width - 28, 0);
			}
			m_engineContext->m_engineSurfaces.push_back(s);
		}

		m_engineContext->m_size = glm::vec2(m_width, m_height);
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

	void Core::onWindowResized(int _x, int _y)
	{
		//m_RT->Initialise(_x, _y);

		glm::vec2 delta = glm::vec2(_x - m_width, _y - m_height);
		m_width = _x;
		m_height = _y;

		float xRatio = delta.x / m_gameContext->m_size.x;
		float yRatio = delta.y / m_gameContext->m_size.y;
		xRatio += 1.0f;
		yRatio += 1.0f;
		m_gameContext->m_size += delta;

		for (std::vector<std::weak_ptr<Surface>>::iterator it = m_gameContext->m_surfaces.begin(); it != m_gameContext->m_surfaces.end(); it++)
		{
			if ((*it).lock()->scaleWithContext)
			{
				if ((*it).lock()->isRender)
				{
					std::shared_ptr<DisplayUI> r = std::static_pointer_cast<DisplayUI>((*it).lock());
					if (r->m_RT)
					{
						r->setSize(r->getSize().x * xRatio, r->getSize().y * yRatio);
					}
				}
				else
				{
					(*it).lock()->m_size = glm::vec2((*it).lock()->m_size.x * xRatio, (*it).lock()->m_size.y * yRatio);
				}
			}
		}


		if (m_engineContext)
		{
			m_engineContext->m_surfaces.clear();
			buildEngineUI();
		}
	}

	std::shared_ptr<Camera> Core::getDefaultCamera()
	{
		if (m_defaultCamera.lock())
		{
			return m_defaultCamera.lock();
		}
		else
		{
			throw Exception("There is no set default camera. The engine requires a default camera specified");			
			return nullptr;
		}
	}

	void Core::setDefaultCamera(std::shared_ptr<Camera> _cam)
	{
		m_defaultCamera = _cam;
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
					int num = std::distance(m_dirLights.begin(), it);
					if (num < m_maxDir)
					{
						(*it).lock()->update(num);
					}
					else
					{
						Console::output(Console::Error, "DirLight Update", "Number of directional lights cannot exceed Engine defined maximum");
					}

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
				it++;
			}
		}
		std::shared_ptr<ShadowCube> random;
		for (std::vector<std::weak_ptr<PointLight>>::iterator it = m_pointLights.begin(); it != m_pointLights.end();)
		{
			try
			{
				if ((*it).lock())
				{
					int num = std::distance(m_pointLights.begin(), it);
					if (num < m_maxPoint)
					{
						(*it).lock()->update(num);
						random = (*it).lock()->getShadowCube();
					}
					else
					{
						Console::output(Console::Error, "PointLight Update", "Number of point lights cannot exceed Engine defined maximum");
					}
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
				it++;
			}
		}		
		if (random)
		{
			for (int i = m_pointLights.size(); i < m_maxPoint; i++) //After updating, we need to fill the Shadowcube array with garbage because it breaks otherwise
			{
				try
				{
					std::string it = std::to_string(i);
					std::string uniform = "in_pointShadowMap[" + it + "]";
					m_lightingSh->setUniform(uniform, random);
				}
				catch (Exception &e)
				{
					Console::output(Console::Error, "pointLight Update", e.message());					
				}
			}
		}


		for (std::vector<std::weak_ptr<SpotLight>>::iterator it = m_spotLights.begin(); it != m_spotLights.end();)
		{
			try
			{
				if ((*it).lock())
				{
					int num = std::distance(m_spotLights.begin(), it);
					if (num < m_maxSpot)
					{
						(*it).lock()->update(num);
					}
					else
					{
						Console::output(Console::Error, "spotLight Update", "Number of spot lights cannot exceed Engine defined maximum");
					}
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
				it++;
			}
		}
	}

	void Core::updateEntities()
	{
		for (int i = 0; i < m_entities.size(); i++) //I have to use int i instead of an iterator because otherwise you can't dynamically create entities here
		{
			try
			{
				if (m_entities[i]->isActive())
				{
					m_entities[i]->tick(); //"Update"
				}
			}
			catch (Exception &e)
			{
				Console::output(Console::Error, m_entities[i]->getTag(), e.message());				
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

	void Core::addDisplay(std::shared_ptr<DisplayUI> _display)
	{			
		_display->m_context = m_gameContext;
		_display->setPosition(0, 0);
		m_gameContext->m_surfaces.push_back(_display);
		orderSurfaces();		
	}

	void Core::addUISurface(std::shared_ptr<Surface> _surface)
	{		
		_surface->m_context = m_gameContext;
		m_gameContext->m_surfaces.push_back(_surface);
		orderSurfaces();		
	}

	std::shared_ptr<Surface> Core::getSurface(int _layer)
	{
		for (std::vector<std::weak_ptr<Surface>>::iterator it = m_gameContext->m_surfaces.begin(); it != m_gameContext->m_surfaces.end(); it++)
		{
			if ((*it).lock()->m_layer == _layer)
			{				
				return (*it).lock();
			}
		}
		Console::output(Console::Warning, "GetSurface", "No surface found on layer: " + _layer);
		return nullptr;
	}

	struct 
	{
		bool operator()(std::weak_ptr<Surface> _a, std::weak_ptr<Surface> _b) const
		{
			return _a.lock()->getLayer() < _b.lock()->getLayer();
		}
	} layerCompare;

	void Core::orderSurfaces()
	{
		std::sort(m_gameContext->m_surfaces.begin(), m_gameContext->m_surfaces.end(), layerCompare);
	}

	void Core::renderScreen()
	{
		//First update the game context
		if (m_gameContext->m_surfaces.size() == 0)
		{
			Console::output(Console::Error, "Core", "No surfaces detected in the scene");
		}
		else
		{			
			for (std::vector<std::weak_ptr<Surface>>::iterator it = m_gameContext->m_surfaces.begin(); it != m_gameContext->m_surfaces.end();)
			{
				if (!(*it).lock()) //It has been deleted by the user already, so we need to remove it from our list too.
				{					
					it = m_gameContext->m_surfaces.erase(it);
				}
				else if ((*it).lock()->getEntity()->isActive())
				{
					if ((*it).lock()->isRender)
					{ 
						std::shared_ptr<DisplayUI> r = std::static_pointer_cast<DisplayUI>((*it).lock());
						if (r->m_RT)
						{
							if (r->m_camera.lock())
							{								
								glBindFramebuffer(GL_FRAMEBUFFER, r->m_RT->getFrameBufID());								
								glClearColor(0.0, 0.0, 0.0, 1.0);
								glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
								glViewport(0, 0, r->m_size.x, r->m_size.y);
								updateLightingShader(r->m_camera.lock(), r->m_size);
								updateSurfaceShader(r->m_RT, r->m_alpha, r->m_col);
								drawScene();
								glBindFramebuffer(GL_FRAMEBUFFER, 0);
								glViewport((*it).lock()->getTruePosition().x, (*it).lock()->getTruePosition().y, (*it).lock()->m_size.x, (*it).lock()->m_size.y);
								glDisable(GL_DEPTH_TEST);
								m_sqShader->draw((*it).lock()->m_screenQuad); //draw a quad with any render textures on it (1 by default)	
								glEnable(GL_DEPTH_TEST);
							}
							else
							{
								Console::output(Console::Error, "Render Surface", "Surface has a render texture but no attached camera");
							}
						}
						else
						{
							Console::output(Console::Error, "Render Surface", "Render Surface has no rendertexture attached");
						}
					}
					else 
					{						
						std::shared_ptr<ImageUI> ui = std::static_pointer_cast<ImageUI>((*it).lock());
						glClearColor(0.0, 0.0, 0.0, 1.0);
						glClear(GL_DEPTH_BUFFER_BIT);						
						glViewport(ui->getTruePosition().x, ui->getTruePosition().y, ui->m_size.x, ui->m_size.y);						
						updateSurfaceShader(ui->m_tex, ui->m_alpha, ui->m_col);
						glDisable(GL_DEPTH_TEST);
						m_sqShader->draw(ui->m_screenQuad); //draw a quad with any render textures on it (1 by default)
						glEnable(GL_DEPTH_TEST);
					}
					it++;
				}				
								
			}
		}

		if (m_engineContext)
		{
			//second, update the Engine Context (for UI and stuff)
			for (std::vector<std::shared_ptr<Surface>>::iterator it = m_engineContext->m_engineSurfaces.begin(); it != m_engineContext->m_engineSurfaces.end(); it++)
			{
				std::shared_ptr<ImageUI> ui = std::static_pointer_cast<ImageUI>((*it));
				glClearColor(0.0, 0.0, 0.0, 1.0);
				glClear(GL_DEPTH_BUFFER_BIT);
				glViewport(ui->getTruePosition().x, ui->getTruePosition().y, ui->m_size.x, ui->m_size.y);
				updateSurfaceShader(ui->m_tex, ui->m_alpha, ui->m_col);
				glDisable(GL_DEPTH_TEST);
				m_sqShader->draw(ui->m_screenQuad); //draw a quad with any render textures on it (1 by default)
				glEnable(GL_DEPTH_TEST);
			}
		}

		SDL_GL_SwapWindow(m_window);
	}
}
