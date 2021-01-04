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
#include "Light.h"



namespace Engine
{
	struct Entity;		
	struct ResourceManager;
	class Camera;
	class Shader;
	class VertexArray;

	struct Core
	{
		friend class Entity;
		//Functions
		static std::shared_ptr<Core> initialise();
		std::shared_ptr<Entity> createEntity();
		void start();
		void loop();	
		void drawScene();
		void drawShadowScene();/* !This has been CREATED as part of the GRAPHICS UNIT! */
		void drawPointShadowScene();/* !This has been CREATED as part of the GRAPHICS UNIT! */
		

		//Not functions		
		std::shared_ptr<Input> m_input;
		std::shared_ptr<ResourceManager> m_rManager;

		//SDL OPENGL and OPENAL STUFF		
		SDL_Window* m_window;
		ALCdevice* m_device;
		ALCcontext* m_context;
		void initialiseAL();
		void initialiseShaders();
		void initialiseSDL();
		float getDeltaTime() { return dTime; }
		std::vector<std::shared_ptr<Entity>> getAllEntities() { return m_entities; }
		
		void updateShader();
		void drawShadowmaps();
		std::shared_ptr<RenderTexture> createRenderTexture();		
		void createScreenQuad();
		std::shared_ptr<Shader> m_sqShader;
		std::shared_ptr<Shader> m_lightingSh;
		std::shared_ptr<Shader> m_shadowSh;
		std::shared_ptr<Shader> m_pointShadowSh;
		std::shared_ptr<Camera> m_camera;

	private:
		std::vector<std::shared_ptr<Entity>> m_entities;		
		int permX, permY;
		bool quit;
		bool restart;
		int width, height;
		int halfX;
		int halfY; 
		int lastX; 
		int lastY; 
		int mouseX, mouseY;
		float dTime;
		long t1;
		bool freeMouse = false;
		
		std::shared_ptr<RenderTexture> m_RT;
		std::shared_ptr<VertexArray> m_screenQuad;

		std::weak_ptr<Core> m_self;
		
		std::vector<std::shared_ptr<DirLight>> m_dirLights; 
		std::vector<std::shared_ptr<SpotLight>> m_spotLights; 
		std::vector<std::shared_ptr<PointLight>> m_pointLights; 
	};
}

#endif