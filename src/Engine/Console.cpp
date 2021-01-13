#include "Console.h"
#include <time.h>

namespace Engine
{	
	std::vector<std::string> Console::m_messageLog;

	void Console::output(type _t, std::string _sender, std::string _text)
	{
		//Reference https://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
		time_t now = time(0);
		struct tm tstruct;
		char buf[80];
		tstruct = *localtime(&now);		
		strftime(buf, sizeof(buf), "%X", &tstruct);
		std::string timeStamp = std::string(buf);
		
		std::string logEntry = "<" + _sender + ">" + "(" + timeStamp + ") '" + _text + "'";

		switch (_t)
		{
			case type::Message:
			{
				logEntry = "[Message]" +  logEntry;
				break;
			}
			case type::Warning:
			{
				logEntry = "[Warning]" + logEntry;
				break;
			}
			case type::Error:
			{
				logEntry = "[Error]" + logEntry;
				break;
			}
			case type::FatalError:
			{
				logEntry = "[FatalError]" + logEntry;
				break;
			}
		}

		bool skip = false;
		for (int i = 0; i < m_messageLog.size(); i++)
		{			
			if (m_messageLog[i].find(_text) != std::string::npos)
			{
				if (m_messageLog[i].find(_sender) != std::string::npos)
				{
					skip = true;
					break;
				}
			}
		}
		if (!skip)
		{
			std::cout << logEntry << std::endl;
			m_messageLog.push_back(logEntry);
		}
	}

	void Console::output(type _t, std::string _sender, std::string _text, bool _duplicates)
	{
		//Reference https://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
		time_t now = time(0);
		struct tm tstruct;
		char buf[80];
		tstruct = *localtime(&now);
		strftime(buf, sizeof(buf), "%X", &tstruct);
		std::string timeStamp = std::string(buf);

		std::string logEntry = "<" + _sender + ">" + "(" + timeStamp + ") '" + _text + "'";

		switch (_t)
		{
		case type::Message:
		{
			logEntry = "[Message]" + logEntry;
			break;
		}
		case type::Warning:
		{
			logEntry = "[Warning]" + logEntry;
			break;
		}
		case type::Error:
		{
			logEntry = "[Error]" + logEntry;
			break;
		}
		case type::FatalError:
		{
			logEntry = "[FatalError]" + logEntry;
			break;
		}
		}

		if (_duplicates)
		{
			std::cout << logEntry << std::endl;
			m_messageLog.push_back(logEntry);
		}
		else
		{
			bool skip = false;
			for (int i = 0; i < m_messageLog.size(); i++)
			{
				if (m_messageLog[i].find(_text) != std::string::npos)
				{
					if (m_messageLog[i].find(_sender) != std::string::npos)
					{
						skip = true;
						break;
					}
				}
			}
			if (!skip)
			{
				std::cout << logEntry << std::endl;
				m_messageLog.push_back(logEntry);
			}
		}
	}

	void Console::output(std::string _text)
	{
		output(type::Message, "Console", _text);
	}

	void Console::printToFile()
	{
		//Reference https://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
		time_t now = time(0);
		struct tm tstruct;
		char buf[80];
		tstruct = *localtime(&now);
		strftime(buf, sizeof(buf), "%F-%H-%M-%S", &tstruct);
		std::string timeStamp = std::string(buf);

		std::fstream file;
		
		file.open(timeStamp + "_Console.log", std::fstream::binary| std::fstream::trunc | std::fstream::out);
		if (file)
		{
			for (int i = 0; i < m_messageLog.size(); i++)
			{
				file << (m_messageLog[i] + '\n');
			}
		}
		else
		{
			int t = 5;
		}
		file.close();
	}
}
