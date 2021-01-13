#ifndef _RESOURCE_H_
#define _RESOURCE_H_

#include <string>
#include <AL/al.h>
#include <AL/alc.h>
#include <vector>
#include <iostream>
#include "Exception.h"
#include "Console.h"

namespace Engine
{
	class Shader;

	/** \brief A Base class for all Resource objects. */
	/** A resource is anything that gets loaded in, such as Texture, VertexArray, Sound.  */
	class Resource
	{
		public:
		/** \brief The path to the resource*/
		std::string m_path;		
	};

	/** \brief The Resource Manager handled the loading in of Resources and saving them in memory	*/
	class ResourceManager
	{
		/** \brief The list of already loaded in Resources*/
		std::vector<std::shared_ptr<Resource>> m_resourceList;

		public:
		/** \brief Loads in a resource, of type T, from a path and returns it*/
		template <typename T>
		std::shared_ptr<T> load(std::string _path)
		{	
			std::shared_ptr<T> rtn = std::make_shared<T>();
			try
			{
				for (int i = 0; i < m_resourceList.size(); i++)
				{
					if (m_resourceList[i]->m_path == _path)
					{
						return std::static_pointer_cast<T>(m_resourceList[i]);
					}
				}
				
				rtn->m_path = _path;
				rtn->load(_path);
				m_resourceList.push_back(rtn);
				
			}
			catch(Exception& e)
			{
				Console::output(Console::Error, "Resource Load", e.message());
			}
			return rtn;
		}
	};

	/** \brief A sound. This can be Ogg or WAV format.	*/
	/** Sounds get played and are usually held by a SoundSource  */
	class Sound : public Resource
	{
		friend class SoundSource;
		public:
		/** \brief Loads in a sound. This gets called by the ResourceManager*/
		void load(std::string path);

		private:
		/** \brief The ID of the sound*/
		ALuint m_id  = 0;	

		/** \brief A function for loading OGG format*/
		void loadOgg(const std::string& fileName, std::vector<char> &buffer, ALenum &format, ALsizei &freq);

		/** \brief A function for loading WAV format*/
		char* loadWav(const std::string & fileName, int &size, ALenum & format, ALsizei & freq);
	};
}

#endif