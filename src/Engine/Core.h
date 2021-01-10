#ifndef _CORE_H_
#define _CORE_H_

#include <memory>
#include <vector>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <random>
#include <time.h>
#include "Input.h"
#include <AL/al.h>
#include <AL/alc.h> 
#include "RenderTexture.h"
//#include "Light.h"


namespace Engine
{
	class Entity;		
	class ResourceManager;	
	class Shader;
	class VertexArray;
	class Camera; 
	class DirLight;
	class SpotLight;
	class PointLight;
	class Surface;
	class Display;
	class UISurface;
	class AudioReceiver;
	class Context;

	struct Core
	{
		friend class Entity;		
		enum RunMode
		{
			Debug, Release
		};


		//Functions
		static std::shared_ptr<Core> initialise(RunMode _mode, glm::vec2 _gameWindowize);
		std::shared_ptr<Entity> createEntity();
		void start();
		void loop();
		std::shared_ptr<InputManager> getInput() { return m_inputManager; }
		float getDeltaTime() { return dTime; }
		std::vector<std::shared_ptr<Entity>> getAllEntities() { return m_entities; }
		//std::shared_ptr<RenderTexture> createRenderTexture();
		void onWindowResized(int _x, int _y);
		std::shared_ptr<Camera> getDefaultCamera();
		void setDefaultCamera(std::shared_ptr<Camera> _cam);
		std::shared_ptr<Display> createDisplay(std::shared_ptr<Camera> _cam, int _layer);
		std::shared_ptr<UISurface> createUISurface(std::shared_ptr<Texture> _tex, int _layer);
		std::shared_ptr<Surface> getSurface(int _layer);
		void orderSurfaces();
		std::shared_ptr<AudioReceiver> getAudioReceiver() { return m_listener.lock(); }



		//std::shared_ptr<Camera> getCurrentCamera();		

		//SDL OPENGL and OPENAL STUFF		
		SDL_Window* m_window;
		ALCdevice* m_device;
		ALCcontext* m_context;
		
		//Not functions	
		std::shared_ptr<ResourceManager> m_rManager;
		std::shared_ptr<Shader> m_sqShader;
		std::shared_ptr<Shader> m_lightingSh;
		std::shared_ptr<Shader> m_shadowSh;
		std::shared_ptr<Shader> m_pointShadowSh;

		

	private:		
		//General variables
		bool quit;
		bool restart;
		int width;
		int height;

		glm::vec2 defaultGameSize = glm::vec2(1024, 1024);
		glm::vec2 defaultWindowSize = glm::vec2(1424, 1052);


		
		//Time variables
		float dTime;
		long t1;		
		
		//My things 
		std::vector<std::shared_ptr<Entity>> m_entities;
		std::shared_ptr<InputManager> m_inputManager;
		//std::vector<std::shared_ptr<Surface>> m_surfaces;
		std::shared_ptr<Context> m_engineContext;
		std::shared_ptr<Context> m_gameContext;


		//Things I just hold references to
		std::weak_ptr<Core> m_self;		
		std::weak_ptr<Camera> m_defaultCamera;
		std::vector<std::weak_ptr<DirLight>> m_dirLights; 
		std::vector<std::weak_ptr<SpotLight>> m_spotLights; 
		std::vector<std::weak_ptr<PointLight>> m_pointLights; 
		std::weak_ptr<AudioReceiver> m_listener;


		//Utility functions that just make the code easier for my eyes
		void drawScene();
		void drawShadowScene();/* !This has been CREATED as part of the GRAPHICS UNIT! */
		void drawPointShadowScene();/* !This has been CREATED as part of the GRAPHICS UNIT! */
		void updateLightingShader(std::shared_ptr<Camera> _cam, glm::vec2 _viewport);
		void updateSurfaceShader(std::shared_ptr<Texture> _tex, float _alpha);
		void initialiseAL();
		void initialiseShaders();
		void initialiseSDL();
		void updateLighting();
		void updateEntities();		
		void drawShadowmaps();
		void buildEngineUI();
		
		void renderScreen();
		//bool layerCompare(std::shared_ptr<Surface> _a, std::shared_ptr<Surface> _b);
		
	};
}

#endif