#include <glm/glm.hpp>
#include "Collision.h"
#include "PhysicsNew.h"


namespace Engine
{
	Collision::Collision(glm::vec3 _cP, glm::vec3 _norm, std::shared_ptr<Entity> _other, std::shared_ptr<Entity> _my)
	{
		m_cP = _cP;
		m_normal = _norm;
		m_other = _other;
		m_my = _my;

		std::shared_ptr<PhysicsObject> rb = _my->getComponent<PhysicsObject>();
		if (rb)
		{
			float invBounce = 0.0f - (1.0f + rb->getBounciness());
			glm::vec3 otherVel;
			float otherMass;

			std::shared_ptr<PhysicsObject> orb = _my->getComponent<PhysicsObject>();
			if (!orb) // If the other object isn't a physics object
			{
				otherVel = glm::vec3(0.0f, 0.0f, 0.0f);
				otherMass = INFINITY;
			}
			else // If it is
			{
				otherVel = orb->getVelocity();
				otherMass = orb->getMass();
			}

			m_velDif = rb->getVelocity() - otherVel;
			m_massCalc = (1.0f / rb->getMass()) + (1.0f / otherMass);
			m_resultForce = glm::dot(invBounce * m_velDif, m_normal) / m_massCalc;
			m_deltaVel = (m_resultForce * m_normal) / rb->getMass();
			std::shared_ptr<Collider> col = _my->getComponent<Collider>();


			if (_my->getComponent<SphereCollider>())
			{
				std::shared_ptr<Transform> trans = _my->getComponent<Transform>();
				m_poC = trans->getPosition() + trans->getScale().y * -m_normal; //The PoC will always be a radius away in the direction of the collision normal
			}
		}
	}

	void Collision::ignoreCollision()
	{		
		m_cP = m_my->getComponent<Transform>()->getPosition();
		m_deltaVel = glm::vec3(0.0f);
	}
}