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
	struct Entity;		
	struct ResourceManager;	
	class Shader;
	class VertexArray;
	class Camera; 
	class DirLight;
	class SpotLight;
	class PointLight;

	struct Core
	{
		friend class Entity;		

		//Functions
		static std::shared_ptr<Core> initialise();
		std::shared_ptr<Entity> createEntity();
		void start();
		void loop();
		std::shared_ptr<InputManager> getInput() { return m_inputManager; }
		float getDeltaTime() { return dTime; }
		std::vector<std::shared_ptr<Entity>> getAllEntities() { return m_entities; }
		std::shared_ptr<RenderTexture> createRenderTexture();
		void resizeWindow(int _x, int _y);
		std::shared_ptr<Camera> getCurrentCamera();		

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
		int width = 1024;
		int height = 1024;
		
		//Time variables
		float dTime;
		long t1;		
		
		//My things
		std::shared_ptr<RenderTexture> m_RT;
		std::shared_ptr<VertexArray> m_screenQuad;
		std::vector<std::shared_ptr<Entity>> m_entities;
		std::shared_ptr<InputManager> m_inputManager;

		//Things I just hold references to
		std::weak_ptr<Core> m_self;
		std::weak_ptr<Camera> m_camera;
		std::vector<std::weak_ptr<DirLight>> m_dirLights; 
		std::vector<std::weak_ptr<SpotLight>> m_spotLights; 
		std::vector<std::weak_ptr<PointLight>> m_pointLights; 


		//Utility functions that just make the code easier for my eyes
		void drawScene();
		void drawShadowScene();/* !This has been CREATED as part of the GRAPHICS UNIT! */
		void drawPointShadowScene();/* !This has been CREATED as part of the GRAPHICS UNIT! */
		void updateShader();
		void initialiseAL();
		void initialiseShaders();
		void initialiseSDL();
		void updateLighting();
		void updateEntities();
		void drawShadowmaps();
		void createScreenQuad();
		void renderScreen();
	};
}

#endif