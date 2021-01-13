#ifndef _CONSOLE_H_
#define _CONSOLE_H_
#include <iostream>
#include <vector>
#include <string>
#include <fstream>


namespace Engine
{
	/** \brief  The Console is how the Engine sends messages to the user.*/
	/** The Console is static, you can access it from any file with Console::output */
	class Console
	{
	public:
		/** \brief This enum is to define what type of message the console should display*/
		enum type
		{
			Message = 0,
			Warning = 1,
			Error = 2,
			FatalError = 3
		};

		/** \brief Output something to the console.*/
		static void output(type _t, std::string _sender, std::string _text);

		/** \brief Output something to the console, use this to allow duplicate messages to be printed. */
		static void output(type _t, std::string _sender, std::string _text, bool _duplicates);

		/** \brief Output a message to the console.*/
		static void output(std::string _text);		

		/** \brief Prints the whole log to a file*/
		static void printToFile();
		

	private:		
		Console() {};
		/** \brief This is a log of all messages the console has sent*/
		static std::vector<std::string> m_messageLog; 
	};
}
#endif