#include "Resource.h"
#include "stb_vorbis.h"
#include "Exception.h"
#include "WavFile.h"


namespace Engine
{
	void Sound::load(std::string path)
	{
		alGenBuffers(1, &m_id);
		ALenum format = 0;
		ALsizei freq = 0;
		std::vector<char> bufferData;
		int size;
		path = "../resources/Sounds/" + path;
		if (path[path.size() - 1] == 'v')
		{
			char* data = loadWav(path, size, format, freq);
			alBufferData(m_id, format, data, static_cast<ALsizei>(size), freq);
		}
		else
		{
			loadOgg(path, bufferData, format, freq);
			alBufferData(m_id, format, &bufferData[0], static_cast<ALsizei>(bufferData.size()), freq);
		}
	}

	char* Sound::loadWav(const std::string & fileName, int & size, ALenum & format, ALsizei & freq)
	{
		int channels = 0;		
		int bps = 0;		
		char* t = WavFile::loadWAV(fileName.c_str(), channels, freq, bps, size);

		if (!t);
		{
			throw Exception("loadWav failed: " + fileName);
		}

		if (channels == 1)
		{
			format = AL_FORMAT_MONO16;
		}
		else
		{
			format = AL_FORMAT_MONO16;
			freq *= 2;
			Console::output(Console::Warning, "Sound", "Sound file is not mono. The quality may be slightly degraded: " + fileName);			
		}			
		
		return t;
	}


	void Sound::loadOgg(const std::string & fileName, std::vector<char>& buffer, ALenum & format, ALsizei & freq)
	{
		int channels = 0;		
		short *output = NULL;
		size_t samples = stb_vorbis_decode_filename(fileName.c_str(), &channels, &freq, &output);
		if (samples == -1)
		{
			throw Exception("loadOgg failed: " + fileName);
		}
		
		if (channels == 1)
		{
			format = AL_FORMAT_MONO16;
		}
		else if (channels == 2)
		{
			format = AL_FORMAT_MONO16;
			freq *= 2;
			Console::output(Console::Warning, "Sound", "Sound file is not mono. The quality may be slightly degraded: " + fileName);			
		}
		else
		{
			throw Exception("loadOgg failed, unsupported number of channels: " + fileName);
		}

		buffer.resize(samples * 2);
		memcpy(&buffer.at(0), output, buffer.size());
		
		free(output);	
	}
}
