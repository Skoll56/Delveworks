#include "Component.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Entity.h"
#include "Input.h"

namespace Engine
{
	void Component::destroy()
	{
		m_delete = true;
	}

	std::shared_ptr<Input> Component::input()
	{
		 return getEntity()->getCore()->m_input; 
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

	void Transform::onTick()
	{
		glm::mat4 modelMat(1.0f); // Model Matrix
		modelMat = glm::translate(modelMat, getPosition()); // Translate by game object's position

		std::shared_ptr<AdvPhysicsObject> aRB = getEntity()->getComponent<AdvPhysicsObject>();
		if (aRB)
		{
			//if (_obj->getTag() != "testbox")
			{
				glm::mat4 rotationMat = glm::mat4(aRB->getRotMat());
				glm::quat rotate = glm::normalize(glm::quat_cast(rotationMat));
				aRB->setRotMat(glm::mat3_cast(rotate));

				modelMat *= glm::mat4_cast(rotate);
			}
			/*else
			{
				glm::mat4 rotMatNew = glm::mat4_cast(aRB->getRotQuat());
				modelMat *= rotMatNew;
			}*/
		}
		else
		{
			glm::vec3 rotation = glm::radians(m_eulerAngles);
			{
				modelMat = glm::rotate(modelMat, rotation.x, glm::vec3(1, 0, 0)); //Rotate by the shape component's rotation
				modelMat = glm::rotate(modelMat, rotation.y, glm::vec3(0, 1, 0));
				modelMat = glm::rotate(modelMat, rotation.z, glm::vec3(0, 0, 1));
			}
		}
		modelMat = glm::scale(modelMat, getScale()); // Scale by shape object's scale
		setModel(modelMat);

		//Reference LearnOpenGL
		m_fwd.x = cos(glm::radians(m_eulerAngles.y)) * cos(glm::radians(m_eulerAngles.x));
		m_fwd.y = sin(glm::radians(m_eulerAngles.x));
		m_fwd.z = sin(glm::radians(m_eulerAngles.y)) * cos(glm::radians(m_eulerAngles.x));
		m_fwd = glm::normalize(-m_fwd); //Idk why it's -fwd but for some reason it works, so, moving along.
		m_right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), m_fwd));
		m_upVec = glm::cross(m_fwd, m_right);
		
	}

	void Transform::onInitialise()
	{
		m_upVec = glm::vec3(0.0f, 1.0f, 0.0f);
	}

}
 