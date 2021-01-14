#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>
#include <glm/glm.hpp>
#include <AL/al.h>
#include "Resource.h"
#include "Input.h"
#include "Core.h"

namespace Engine
{
	class Entity;	
	class Transform;
	class Keyboard;
	class Collision;	
	class Collider;

	/** \brief  The Base class for all Components in the Engine. */
	/** Most things, including user defined-modules, typically inherit from this */
	class Component
	{
		friend class Entity;

	public:
		/** \brief Call this to destroy a component at the end of the frame*/
		void destroy();

		/** \brief Returns the Entity which contains this Component*/
		std::shared_ptr<Entity> getEntity() { return m_entity.lock(); }

		/** \brief Returns the Transform component attached to the same object as this*/
		std::shared_ptr<Transform> transform() { return m_transform.lock(); }		

		template <typename T>
		/** \brief Returns an InputDevice of type T from the InputManager*/
		std::shared_ptr<T> getInputDevice()
		{
			return getCore()->getInput()->getDevice<T>();
		}

		/** \brief Returns a reference to the Core*/
		std::shared_ptr<Core> getCore();

		/** \brief A "This" pointer*/
		std::weak_ptr<Component> m_self;
	protected:
		/** \brief Initialise a component*/
		/** Components can also specify arguments to go into this function. You pass them in when you AddComponent. */
		virtual void onInitialise();

		/** \brief Is called every frame the parent object in Colliding for. Contains the collision info.*/
		virtual void onCollision(std::shared_ptr<Collision> _col);

		/** \brief Is called on the first frame of collision. Contains the collision info.*/
		virtual void onCollisionEnter(std::shared_ptr<Collision> _col);

		/** \brief Is called on the last frame of a collision. Contains a pointer to the other Entity.*/
		virtual void onCollisionExit(std::shared_ptr<Entity> _other);

		/** \brief This is called once per frame*/
		virtual void onTick();
		
		/** \brief This is called just before a Component gets destroyed*/
		virtual void onDestroy();

		/** \brief A reference to the entity this is attached to*/
		std::weak_ptr<Entity> m_entity;

		/** \brief A reference to the Transform attached to the Entity*/
		std::weak_ptr<Transform> m_transform;

		/** \brief Is set to true when the object is due to be deleted*/
		bool m_delete = false;
	};

	/** \brief  The Transform Component contains Translation, Rotation and Scale data*/
	/** This is AUTOMATICALLY added to any Entity when it is created. All Entities are required to have Transforms. */
	class Transform : public Component
	{
		friend class Entity;
		friend class MeshRenderer;

		public:
		void setPosition(float _posX, float _posY, float _posZ) { m_position.x = _posX; m_position.y = _posY; m_position.z = _posZ; }
		void setPosition(glm::vec3 _value) { m_position = _value; }
		glm::vec3 getPosition() { return m_position; }
		glm::vec3 getScale() { return m_scale; }
		void setScale(glm::vec3 _scale) { m_scale = _scale; }
		glm::vec3 getSize() { return m_size; }
		/** \brief Returns the Model Matrix of the Entity, which contains its translation, rotation and scale data in Matrix format*/
		glm::mat4 getModel() { return m_model; }

		/** \brief Sets the Model Matrix of the Entity. Only set this if you know what you are doing, or you may see unexpected results.*/
		void setModel(glm::mat4 _model) { m_model = _model; }

		/** \brief Update*/
		void onTick();		
		
		/** \brief Rotates an object around an axis, by an amount in degrees.*/
		void rotate(glm::vec3 _axis, float _amt) { m_eulerAngles += _amt * _axis; }

		/** \brief Gets an 'Forward' vector (A Vector in the direction the object is facing)*/
		glm::vec3 getFwd() { return m_fwd; }

		/** \brief Gets an 'Up' vector (A vector perpendicular to the forward vector, pointing in the local vertical axis.*/
		glm::vec3 getUp() { return m_upVec; }
		
		/** \brief Gets an 'Up' vector (A vector perpendicular to the up vector, pointing in the local horizontal axis.*/
		glm::vec3 getRight() { return m_right; }
		
		glm::vec3 m_position = glm::vec3(0.0f);

		/** \brief The object's rotation in Euler Angles, degrees, XYZ format*/
		glm::vec3 m_eulerAngles = glm::vec3(0.0f);
		
		/** \brief A "This" pointer*/
		std::weak_ptr<Transform> m_self;
	private:	
		/** \brief Initialise*/
		void onInitialise();

		/** \brief Scale*/
		glm::vec3 m_scale = glm::vec3(1.0f);
		/** \brief Size refers to the size of the MESH/OBJ in generic units. */
		glm::vec3 m_size = glm::vec3(1.0f); //TODO: Calculate this yourself

		/** \brief The Model Matrix*/
		glm::mat4 m_model = glm::mat4(1.0f);

		/** \brief Forward Vector*/
		glm::vec3 m_fwd;

		/** \brief Up Vector*/
		glm::vec3 m_upVec;

		/** \brief Right vector*/
		glm::vec3 m_right;
	};

	/** \brief  A Sound Source holds and plays sounds*/
	class SoundSource : public Component
	{
		friend class Entity;
		public:
		/** \brief Update*/
		void onTick();

		/** \brief Call this to play the stored sound*/
		void Play(float _vol);

		/** \brief Set if the sound should loop*/
		void setLoop(bool _loop) { m_loop = _loop; }

		/** \brief Set if the component should destroy itself after it plays*/
		void setDestroyOnPlay(bool _destroy) { m_destroyOnPlay = _destroy; }

		/** \brief A "this" pointer*/
		std::weak_ptr<SoundSource> m_self;

		private:
		/** \brief Initialise*/
		void onInitialise(std::shared_ptr<Sound> _sound);

		/** \brief An ID for OpenAL*/
		ALuint m_id = 0;

		/** \brief Tracks if it has played yet or not*/
		bool m_played = false;

		/** \brief The sound will play after it finishes if 'Loop' is true*/
		bool m_loop = false;

		/** \brief The component destroys itself after playing if this is true*/
		bool m_destroyOnPlay = false;		
	};

	/** \brief  A set of "ears". This is used to hear sounds.*/
	/** You must have exactly one of these in any scene. If you add multiple, only the last entry will detect sound. */
	class AudioReceiver : public Component
	{
		public:
		/** \brief  A "This" pointer*/
		std::weak_ptr<AudioReceiver> m_self;
	};
}

#endif
