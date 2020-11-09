#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <exception>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "stb_image.h"
#include "Input.h"
#include "Scene.h"
#include "Utility.h"
#include "Camera.h"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

SDL_Window* initialise();


int main(int argc, char *argv[])
{
	SDL_Window *window = initialise();
	bool quit = false;

	while (!quit)
	{
		bool restart = false;
		Scene* scene = new Scene;
	
		scene->createStartScene();
		///VertexBuffer positions
		//shape->m_vBOp->add(glm::vec3(0.0f, 0.5f, 0.0f));
		//shape->m_vBOp->add(glm::vec3(-0.5f, -0.5f, 0.0f));
		//shape->m_vBOp->add(glm::vec3(0.5f, -0.5f, 0.0f));

		///VertexBuffer texCoords
		//shape->m_vBOt->add(glm::vec2(0.5f, 0.0f));
		//shape->m_vBOt->add(glm::vec2(0.0f, 1.0f));
		//shape->m_vBOt->add(glm::vec2(1.0f, 1.0f));	

		///VAO
		//shape->m_vAO->setBuffer("in_Position", shape->m_vBOp);
		//shape->m_vAO->setBuffer("in_TexCoord", shape->m_vBOt);
		int width, height;
		int halfX = WINDOW_WIDTH /2;
		int halfY = WINDOW_HEIGHT / 2;
		int lastX = halfX;
		int lastY = halfY;
		int mouseX, mouseY;
		long t1 = SDL_GetTicks();
		int permX, permY;

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		SDL_ShowCursor(false);


		while (!quit && !restart) // Game Loop
		{
			// Re-initialise per-frame variables
			float time = SDL_GetTicks();
			float dTime = (time - t1) / 1000.0f;
			t1 = time;
			SDL_Event event = { 0 };

			SDL_GetMouseState(&mouseX, &mouseY);
			scene->getInput()->m_xOffset = mouseX - lastX;
			scene->getInput()->m_yOffset = mouseY - lastY;		
			
			//Re-establish window-size to allow stretching and re-sizing
			SDL_GetWindowSize(window, &width, &height);
			utility::onEveryFrame(scene, dTime);				
			if (scene->getInput()->m_restart)
			{
				restart = true;
			}
			
			glViewport(0, 0, width, height);			

			//Set the clear-colour for the screen and clear it
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);
			glClearColor(0.0, 0.0, 0.0, 0.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			

			//Set the active texture buffer
			glActiveTexture(GL_TEXTURE0 + 1);

			scene->update(dTime);
			

			SDL_GL_SwapWindow(window);	
			
			

			
			quit = scene->getInput()->takeInput(event); //Handles the input, and returns a 'quit' value to see if the program should end
			
			float targetTime = 1.0f / 60.f;
			if (targetTime > dTime)
			{
				SDL_Delay((targetTime - dTime) * 1000.0f);
			}

			if (dTime > 100.0f) { dTime = 100.0f; }
			
			bool changed = false;
			
			if (mouseX > width - 50)
			{
				SDL_WarpMouseInWindow(window, 50, mouseY);
				changed = true;				
			}
			else if (mouseX < 50)
			{
				SDL_WarpMouseInWindow(window, width - 50, mouseY);
				changed = true;
			}

			if (mouseY > height - 50)
			{
				SDL_WarpMouseInWindow(window, mouseX, 50);
				changed = true;
			}
			else if (mouseY < 50)
			{
				SDL_WarpMouseInWindow(window, height - 50, mouseX);
				changed = true;
			}
			if (changed) { SDL_GetMouseState(&mouseX, &mouseY); }
			lastX = mouseX;
			lastY = mouseY;
		}
	}
  SDL_DestroyWindow(window); // DESTROY THAT WINDOW. STRIKE IT DOWN. DEWIT.
  SDL_Quit();
  return 0;
}


SDL_Window* initialise() // Starts the window and initialises a lot of the SDL/GLEW stuff
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		throw std::exception();
	}

	SDL_Window *window = SDL_CreateWindow("Physics/Graphics Engine",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	if (!SDL_GL_CreateContext(window))
	{
		throw std::exception();
	}

	if (glewInit() != GLEW_OK)
	{
		throw std::exception();
	}
	return window;
}