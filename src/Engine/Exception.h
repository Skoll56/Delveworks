#include <exception>
#include <string>

#ifndef EXCEPTION_H
#define EXCEPTION_H

namespace Engine
{
	class Exception : public std::exception
	{
	public:
		Exception(std::string _sender, const std::string& _text);
		Exception();
		virtual ~Exception() throw();
		std::string message() const throw();
	private:
		std::string m_message;
	};

	class FatalException : public Exception
	{
	public:
		FatalException(std::string _sender, const std::string& _text);
		virtual ~FatalException() throw();
		std::string message() const throw();
	private:
		std::string m_message;
	
	};

}

#endif