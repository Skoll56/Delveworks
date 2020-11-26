#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <exception>

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