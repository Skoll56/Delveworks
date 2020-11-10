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
	class Resource;

	struct Core
	{
		//Functions
		static std::shared_ptr<Core> initialise();
		std::shared_ptr<Entity> createEntity();
		void start();
		void loop();		
		Scene* createScene();	
		void LoadResources();


		//Not functions
		Scene* m_scene;
		Input* m_input;
		std::shared_ptr<ResourceManager> m_rManager = std::make_shared<ResourceManager>();
		


		//SDL OPENGL and OPENAL STUFF		
		SDL_Window* m_window;
		ALCdevice* m_device;
		ALCcontext* m_context;
		void initialiseAL();
		void initialiseSDL();

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