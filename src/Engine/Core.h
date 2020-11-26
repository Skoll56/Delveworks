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


namespace Engine
{
	struct Entity;		
	struct ResourceManager;
	class Camera;
	class Shader;
	class VertexArray;

	struct Core
	{
		//Functions
		static std::shared_ptr<Core> initialise();
		std::shared_ptr<Entity> createEntity();
		void start();
		void loop();	
		void drawScene();
		

		//Not functions		
		Input* m_input;
		std::shared_ptr<ResourceManager> m_rManager;

		//SDL OPENGL and OPENAL STUFF		
		SDL_Window* m_window;
		ALCdevice* m_device;
		ALCcontext* m_context;
		void initialiseAL();
		void initialiseSDL();
		float getDeltaTime() { return dTime; }
		std::vector<std::shared_ptr<Entity>> getAllEntities() { return m_entities; }
		std::shared_ptr<Shader> m_lightingSh;
		void updateShader();
		std::shared_ptr<RenderTexture> createRenderTexture();
		void createScreenQuad();

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
		
		std::vector<std::shared_ptr<RenderTexture>> m_RTs;
		VertexArray* m_screenQuad;
		std::shared_ptr<Shader> m_sqShader;

		std::weak_ptr<Core> m_self;
		std::shared_ptr<Camera> m_camera;
	};
}

#endif