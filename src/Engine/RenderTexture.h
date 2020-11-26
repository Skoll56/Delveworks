#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <exception>


//RENDER TEXTURES ARE A PART OF THE GRAPHICS UNIT AND WERE ADDED AS A FIRST STEP TOWARDS SHADOWS
namespace Engine
{
	class RenderTexture
	{
		public:
		void Initialise();
		

		
		GLuint fBufID = 0;
		GLuint fBufTexID = 0;
		GLuint rBufObjID = 0;
	};
}