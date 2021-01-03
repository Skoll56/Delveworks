#include "Exception.h"
#include "Console.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <AL/al.h>
#include <AL/alc.h> 


namespace Engine
{
	Exception::Exception(std::string _sender, const std::string & _text)
	{
		m_message = _text;
		Console::output(Console::Error, _sender, _text);
	}
	Exception::Exception()
	{
		m_message = "Exception thrown";
		Console::output(Console::Error, "Exception", m_message);
	}

	Exception::~Exception() throw()
	{
	}

	std::string Exception::message() const throw()
	{
		return m_message;
	}


	FatalException::FatalException(std::string _sender, const std::string & _text)
	{
		m_message = _text;
		Console::output(Console::FatalError, _sender, _text);
		SDL_Quit();

		#if EMSCRIPTEN
		emscripten_cancel_main_loop();
		#endif
		alcMakeContextCurrent(NULL);		
	}
	FatalException::~FatalException() throw()
	{
		m_message = "Fatal Exception thrown";
		Console::output(Console::FatalError, "Exception", m_message);

		SDL_Quit();

		#if EMSCRIPTEN
		emscripten_cancel_main_loop();
		#endif
		alcMakeContextCurrent(NULL);
	}

	std::string FatalException::message() const throw()
	{
		return m_message;
	}

}