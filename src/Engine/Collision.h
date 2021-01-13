#ifndef _COLLISION_H_
#define _COLLISION_H_
#include <string>
#include "Engine.h"


namespace Engine
{
	/** \brief  A class that contains all the information about a Collision.*/
	/** One of these gets generated every time there is a collision */
	class Collision
	{
	public:
		
		Collision(glm::vec3 _cP, glm::vec3 _norm, std::shared_ptr<Entity> _other, std::shared_ptr<Entity> _my);
		/** \brief This function can be called BEFORE the collision is 'Resolved' to negate the Physics reaction*/
		void ignoreCollision();

		/** \brief The 'Other' object is a reference to the Collider which the PhysicsObj hit*/
		std::shared_ptr<Entity> m_other;

		/** \brief The 'My' object is a reference to the PhysicsObj which created this Collision*/
		std::shared_ptr<Entity> m_my;

		/** \brief Point of contact between the two Colliders*/
		glm::vec3 m_poC; 

		/** \brief Position of the 'My' when it collided*/
		glm::vec3 m_cP; 

		/** \brief  The normal of the collision*/
		glm::vec3 m_normal; 

		/** \brief The result force (In newtons) of the collision*/
		float m_resultForce;
		
		/** \brief The difference in Velocity between the two collided objects*/
		glm::vec3 m_velDif;		
		
		/** \brief The delta in velocity that the 'My' object that will happen before and after the collision*/
		glm::vec3 m_deltaVel; 

		private:
		float m_massCalc;
	};

	/** \brief A class that holds all the information about a RayCollision*/
	class RayCollision 
	{
		public:
		/** \brief The entity the ray hit*/
		std::shared_ptr<Entity> m_hitObj;

		/** \brief The direction the ray was travelling*/
		glm::vec3 m_rayDir;

		/** \brief The origin of the ray*/
		glm::vec3 m_origin;

		/** \brief The user-defined tag of the Ray*/
		std::string m_rayTag;

		/** \brief Exactly where in Worldspace the ray collided*/
		glm::vec3 m_hitPoint;

		/** \brief Whether the ray collided or not*/
		bool m_hit;
	};
}

#endif