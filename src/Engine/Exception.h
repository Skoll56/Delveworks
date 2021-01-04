#include <exception>
#include <string>

#ifndef EXCEPTION_H
#define EXCEPTION_H

namespace Engine
{
	class Exception : public std::exception
	{
	public:
		Exception(const std::string& _text);
		Exception();
		virtual ~Exception() throw();
		std::string message() const throw();
	private:
		std::string m_message;
	};

	
}

#endif