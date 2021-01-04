#include "Exception.h"
#include "Console.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <AL/al.h>
#include <AL/alc.h> 


namespace Engine
{
	Exception::Exception(const std::string & _text)
	{
		m_message = _text;		
	}
	Exception::Exception()
	{
		m_message = "Exception thrown";		
	}

	Exception::~Exception() throw()
	{
	}

	std::string Exception::message() const throw()
	{
		return m_message;
	}
}