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
		path = "../resources/Sounds/" + path;
		if (path[path.size() - 1] == 'v')
		{
			LoadWav(path, bufferData, format, freq);
		}
		else
		{
			loadOgg(path, bufferData, format, freq);
		}
		
		alBufferData(m_id, format, &bufferData[0], static_cast<ALsizei>(bufferData.size()), freq);
	}

	//The Wav File loader was provided by Adam Stark: https://github.com/adamstark/AudioFile and doesn't work at all
	void Sound::LoadWav(const std::string & fileName, std::vector<char>& buffer, ALenum & format, ALsizei & freq)
	{
		WavFile<float> wav;		
		wav.load(fileName);
	}

	void Sound::loadOgg(const std::string & fileName, std::vector<char>& buffer, ALenum & format, ALsizei & freq)
	{
		int channels = 0;
		int sampleRate = 0;
		short *output = NULL;
		size_t samples = stb_vorbis_decode_filename(fileName.c_str(), &channels, &sampleRate, &output);
		if (samples == -1)
		{
			throw Exception("loadOgg failed: " + fileName);
		}
		
		if (channels == 1)
		{
			format = AL_FORMAT_MONO16;
		}
		else
		{
			format = AL_FORMAT_STEREO16;
			Console::output(Console::Warning, "Sound", "Sound file is not mono. Spatial sound is not supported for stereo files: " + fileName);
			//format = AL_FORMAT_MONO16;
		}
		
		freq = sampleRate;
		buffer.resize(samples * 2);
		memcpy(&buffer.at(0), output, buffer.size());
		
		free(output);	
	}
}
