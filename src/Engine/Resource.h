#ifndef _RESOURCE_H_
#define _RESOURCE_H_

#include <string>
#include <AL/al.h>
#include <AL/alc.h>
#include <vector>

namespace Engine
{
	class Shader;

	class Resource
	{
	public:
		std::string m_path;		
	};

	class ResourceManager
	{
		std::vector<std::shared_ptr<Resource>> m_resourceList;

	public:
		template <typename T>
		std::shared_ptr<T> load(std::string _path)
		{	
			for (int i = 0; i < m_resourceList.size(); i++)
			{
				if (m_resourceList[i]->m_path == _path)
				{
					return std::static_pointer_cast<T>(m_resourceList[i]);
				}
			}
			std::shared_ptr<T> rtn = std::make_shared<T>();
			rtn->m_path = _path;
			rtn->load(_path);			
			m_resourceList.push_back(rtn);
			return rtn;
		}


	};

	class Sound : public Resource
	{
		friend class SoundSource;

	private:
		ALuint m_id  = 0;
		void load(const std::string& path);
		void loadOgg(const std::string& fileName, std::vector<char> &buffer, ALenum &format, ALsizei &freq);
	};
}

#endif