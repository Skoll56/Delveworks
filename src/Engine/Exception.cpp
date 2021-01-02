#include "Exception.h"

namespace Engine
{
	Exception::Exception(const std::string & _text)
	{
		m_message = _text;
	}
	Exception::~Exception() throw()
	{
	}
	std::string Exception::message() const throw()
	{
		return m_message;
	}
}