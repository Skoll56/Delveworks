#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <exception>
#include "Texture.h"

//RENDER TEXTURES ARE A PART OF THE GRAPHICS UNIT AND WERE ADDED AS A FIRST STEP TOWARDS SHADOWS
namespace Engine
{
	class RenderTexture : public Texture
	{
		public:
		virtual void Initialise();
			
		GLuint fBufID = 0;		
		GLuint rBufObjID = 0;
	};

	class ShadowMap : public RenderTexture
	{
	public:
		void Initialise();
	};
}