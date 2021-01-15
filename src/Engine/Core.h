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
	class Entity;		
	class ResourceManager;	
	class Shader;
	class VertexArray;
	class Camera; 
	class DirLight;
	class SpotLight;
	class PointLight;
	class Surface;
	class DisplayUI;
	class ImageUI;
	class AudioReceiver;
	class Context;

	/** \brief  The Core of the engine. The "world" per se.*/
	/** The Core controls pretty much everything. The user needs to initialise this before doing anything. */
	class Core
	{
		friend class Entity;		
		friend class Surface;
		friend class DisplayUI;
		friend class ImageUI;
		friend class ButtonUI;
		friend class InputManager;
		friend class DirLight;
		friend class SpotLight;
		friend class PointLight;

		public:
		/** \brief This is the mode that the engine will run in*/
		/** Debug mode comes with a UI Console and an EngineUI instead of just the game Context */
		enum RunMode
		{
			Debug, Release
		};

		//Functions
		/** \brief Initialise the Core with a RunMode and a size for the Game's Context*/
		static std::shared_ptr<Core> initialise(RunMode _mode, glm::vec2 _gameWindowize);

		/** \brief Create an entity and return a reference to it*/
		std::shared_ptr<Entity> createEntity();
		
		/** \brief Return a reference to the InputManager*/
		std::shared_ptr<InputManager> getInput() { return m_inputManager; }

		/** \brief Get the time since last frame (Locked to 0.1f maximum)*/
		float getDeltaTime() { return m_dTime; }

		/** \brief Get the number of ticks since the Engine initialised*/
		float getTime() { return m_t1; }

		/** \brief Returns a list of all entities in the scene*/
		std::vector<std::shared_ptr<Entity>> getAllEntities() { return m_entities; }

		/** \brief Returns the default Camera*/
		/** One camera in the scene needs to be the 'default' camera for Directional shadowmaps to centre around */
		std::shared_ptr<Camera> getDefaultCamera();

		/** \brief Sets the default Camera*/
		void setDefaultCamera(std::shared_ptr<Camera> _cam);

		/** \brief Returns a reference to the Game Context*/
		std::shared_ptr<Context> getContext() { return m_gameContext; }

		/** \brief Returns a Surface on a given layer.*/
		std::shared_ptr<Surface> getSurface(int _layer);

		/** \brief Orders the Surface list by layer*/
		void orderSurfaces();

		/** \brief Returns the scene's one audio receiever*/
		std::shared_ptr<AudioReceiver> getAudioReceiver() { return m_listener.lock(); }

		/** \brief Loads something from file*/
		template <typename T>
		std::shared_ptr<T> load(std::string _path)
		{
			return m_rManager->load<T>(_path);
		}

		/** \brief Starts the Core loop. This should be called at the end of the user's Main function.*/
		void start();
		

	private:		
		//General variables
		/** \brief Marks if the engine should quit*/
		bool m_quit;		

		/** \brief The width of the window*/
		int m_width;

		/** \brief The height of the window*/
		int m_height;	
		
		//Time variables
		/** \brief Time since last frame (Maximum 0.1f)*/
		float m_dTime;

		/** \brief Time since Engine started*/
		long m_t1;

		/** \brief The maximum number of directional lights permitted */
		int m_maxDir = 2;

		/** \brief The maximum number of point lights permitted */
		int m_maxPoint = 5;
		
		/** \brief The maximum number of spot lights permitted */
		int m_maxSpot = 5;

		//SDL OPENGL and OPENAL STUFF		
		SDL_Window* m_window;
		ALCdevice* m_device;
		ALCcontext* m_context;
		
		//My things 
		/** \brief A list of all Entities in the scene*/
		std::vector<std::shared_ptr<Entity>> m_entities;

		/** \brief The InputManager*/
		std::shared_ptr<InputManager> m_inputManager;	

		/** \brief The Game's Context*/
		std::shared_ptr<Context> m_engineContext;

		/** \brief The Engine's Context*/
		std::shared_ptr<Context> m_gameContext;
		
		/** \brief The loader for Resource.*/
		std::shared_ptr<ResourceManager> m_rManager;

		/** \brief The Shader for rendering to a screen quad (Used in RenderTexture for DisplayUI)*/
		std::shared_ptr<Shader> m_sqShader;

		/** \brief The common lighting Shader for most objects*/
		std::shared_ptr<Shader> m_lightingSh;

		/** \brief The Shader used to draw ShadowMap objects (Used by DirLight and SpotLight)		*/
		std::shared_ptr<Shader> m_shadowSh;

		/** \brief The Shader used to draw ShadowCube objects (Used by PointLight)*/
		std::shared_ptr<Shader> m_pointShadowSh;

		//Things I just hold references to
		/** \brief A "This" pointer*/
		std::weak_ptr<Core> m_self;	

		/** \brief The default Camera*/
		/** One camera in the scene needs to be the 'default' camera for Directional shadowmaps to centre around */
		std::weak_ptr<Camera> m_defaultCamera;

		/** \brief A list of Directional Lights in the scene*/
		std::vector<std::weak_ptr<DirLight>> m_dirLights; 

		/** \brief A list of all Spot Lights in the scene*/
		std::vector<std::weak_ptr<SpotLight>> m_spotLights; 

		/** \brief A list of all Point Lights in the scene*/
		std::vector<std::weak_ptr<PointLight>> m_pointLights; 

		/** \brief The listener in the scene*/
		std::weak_ptr<AudioReceiver> m_listener;

		//Utility functions that just make the code easier for my eyes
		/** \brief Draws the scene by iterating through every MeshRenderer*/
		void drawScene();

		/** \brief Draws the scene using the Shadow Shader*/
		void drawShadowScene();/* !This has been CREATED as part of the GRAPHICS UNIT! */

		/** \brief Draws the scene using the Point Light Shadow Shader*/
		void drawPointShadowScene();/* !This has been CREATED as part of the GRAPHICS UNIT! */

		/** \brief Passes in variables that the Lighting Shader needs per frame*/
		void updateLightingShader(std::shared_ptr<Camera> _cam, glm::vec2 _viewport);

		/** \brief Passes in variables that the Screen Quad Shader needs per frame*/
		void updateSurfaceShader(std::shared_ptr<Texture> _tex, float _alpha, glm::vec3 _color);

		/** \brief Initialises OpenAL*/
		void initialiseAL();

		/** \brief Loads in all the default Shaders*/
		void initialiseShaders();

		/** \brief Initialises SDL*/
		void initialiseSDL();

		/** \brief Updates all the lights in the scene with per-frame values.*/
		void updateLighting();

		/** \brief Calls OnTick on every Component within an Entity. */
		void updateEntities();

		/** \brief Draws all the ShadowMap objects*/
		void drawShadowmaps();

		/** \brief Builds the Engine's UI*/
		void buildEngineUI();

		/** \brief Renders to any DisplayUI objects within the game Context*/
		void renderScreen();
		
		/** \brief This is the main loop of the engine*/
		void loop();
		
		//Private functions called from other classes
		/** \brief Adds a DisplayUI to the Core list*/
		void addDisplay(std::shared_ptr<DisplayUI> _display);

		/** \brief Adds a Surface to the Core list*/
		void addUISurface(std::shared_ptr<Surface> _display);	

		/** \brief Gets called when the SDL window is stretched*/
		void onWindowResized(int _x, int _y);		
	};
}

#endif