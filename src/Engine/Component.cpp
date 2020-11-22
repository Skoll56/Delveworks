#include "Component.h"


namespace Engine
{
	void Component::destroy()
	{
		m_delete = true;
	}

	void Component::onInitialise()
	{
	}

	void Component::onTick()
	{
	}

	void Component::onRender()
	{
	}
	void Component::onDestroy()
	{
	}

	void SoundSource::onInitialise(std::shared_ptr<Sound> _sound)
	{
		alGenSources(1, &m_id);		
		//alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
		//alSource3f(m_id, AL_POSITION, 0.0f, 0.0f, 0.0f);
		alSourcei(m_id, AL_BUFFER, _sound->m_id);		
	}

	void SoundSource::Play()
	{
		alSourcePlay(m_id);
		m_played = true;
	}

	void SoundSource::onTick()
	{
		if (m_played)
		{
			ALint state = 0;
			alGetSourcei(m_id, AL_SOURCE_STATE, &state);
			if (state == AL_STOPPED)
			{
				if (m_loop)
				{
					alSourcePlay(m_id);
				}
				else if (m_destroyOnPlay)
				{
					destroy();
				}
			}
		}

	}

}
 