#include <string>
#include <AL/al.h>
#include <AL/alc.h>
#include <vector>

namespace Engine
{

	class Resource
	{

	};

	class Sound : public Resource
	{
		friend class SoundSource;

	private:
		ALuint m_id  = 0;
		void onLoad(const std::string& path);
		void loadOgg(const std::string& fileName, std::vector<char> &buffer, ALenum &format, ALsizei &freq);
	};
}