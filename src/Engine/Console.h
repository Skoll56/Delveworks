#include <iostream>
#include <vector>
#include <string>

namespace Engine
{
	class Console
	{
	public:
		enum type
		{
			Message = 0,
			Warning = 1,
			Error = 2,
			FatalError = 3
		};

		static void output(type _t, std::string _sender, std::string _text);
		static void message(std::string _text);		

	private:		
		Console() {};
	};
}