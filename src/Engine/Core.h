#include <memory>
#include <vector>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <random>
#include <time.h>
#include "Scene.h"
#include "Input.h"
#include <AL/al.h>
#include <AL/alc.h> 


namespace Engine
{
	struct Entity;	
	struct Core
	{
		static std::shared_ptr<Core> initialise();
		std::shared_ptr<Entity> createEntity();
		void start();
		void loop();
		void initialiseSDL();
		Scene* createScene();
		void initialiseAL();

		Input* m_input;
		SDL_Window* m_window;
		Scene* m_scene;		
		ALCdevice* m_device;
		ALCcontext* m_context;

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
		long t1;
	};
}