#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include "Component.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
namespace Engine
{
	class Collider;
	class BoxCollider;
	class SphereCollider;
	class MeshCollider;
	class PlaneCollider;
	class RayCollision;
	class Collision;

	/** \brief A base class for all Physics Objects.*/	
	/** This can be instantiated should the user want to manually do collision checks on an object without making it fully respond to Physics */
	class PhysicsEventUser : public Component
	{
		friend class Entity;

		public:
		/** \brief Update*/
		void onTick();

		/** \brief Checks for any collisions*/
		bool checkForCollisions(float _dT, std::vector<std::shared_ptr<Entity>> _other, glm::vec3 _c1);
		
		/** \brief Resets the collision list (empties it)*/
		void resetCollisions();

		/** \brief Sets the collided value*/
		void setCollided(bool _val) { m_collided = _val; }

		/** \brief Gets if this object has collided or not*/
		bool collided() { return m_collided; }

		/** \brief Set whether this object is floored or not*/
		void setFloored(bool _val) { m_floored = _val; }

		/** \brief Returns if this object is floored*/
		bool isFloored() { return m_floored; }

		/** \brief Checks box versus plane collision*/
		bool boxToPlane(std::shared_ptr<BoxCollider> _my, std::shared_ptr<PlaneCollider> _other, glm::vec3 _c1);

		/** \brief Checks tri versus tri collisions*/
		int coplanar_tri_tri(float N[3], float V0[3], float V1[3], float V2[3],
			float U0[3], float U1[3], float U2[3]);

		/** \brief Checks tri versus tri collisions*/
		int NoDivTriTriIsect(float V0[3], float V1[3], float V2[3],
			float U0[3], float U1[3], float U2[3]);

		/** \brief Checks tri versus box collisions*/
		int triBoxOverlap(float boxcenter[3],
			float boxhalfsize[3], float triverts[3][3]);

		/** \brief Checks box versus plane collisions*/
		int planeBoxOverlap(float normal[3], float d, float maxbox[3]);

		/** \brief Determines the distance from a point to a plane (along the normal)*/
		float distanceToPlane(glm::vec3 &n, glm::vec3 &p, glm::vec3 &q);

		/** \brief Check for box versus box collisions*/
		bool boxToBox(std::shared_ptr<BoxCollider> _my, std::shared_ptr<BoxCollider> _other, glm::vec3 _c1);

		/** \brief Check for mesh versus mech collisions (very expensive)*/
		bool meshToMesh(std::shared_ptr<MeshCollider> _my, std::shared_ptr<Collider> _other, glm::vec3 _c1);

		/** \brief Check for sphere versus plane collisions*/
		bool sphereToPlane(std::shared_ptr<SphereCollider> _my, std::shared_ptr<PlaneCollider> _other, glm::vec3 _c1);

		/** \brief Check for sphere versus box collisions*/
		bool sphereToBox(std::shared_ptr<SphereCollider> _my, std::shared_ptr<BoxCollider> _other, glm::vec3 _c1);

		/** \brief Check for sphere versus sphere collisions*/
		bool sphereToSphere(std::shared_ptr<SphereCollider> _my, std::shared_ptr<SphereCollider> _other);

		/** \brief Check for box versus sphere collisions*/
		bool boxToSphere(std::shared_ptr<BoxCollider> _my, std::shared_ptr<SphereCollider> _other, glm::vec3 _c1);

		/** \brief Check for an overlap between a sphere and box collider*/
		bool sphereBoxOverlap(std::shared_ptr<SphereCollider> _sphere, std::shared_ptr<BoxCollider> _box);

		/** \brief Rounds a normals so if it's close to 1, -1 or 0 it rounds it.*/
		glm::vec3 roundNormals(glm::vec3 _n);

		/** \brief Returns the permanent contact point*/
		glm::vec3 getPermCP() { return m_permCP; }

		/** \brief Sets the permanent contact point*/
		void setPermCP(glm::vec3 _pos) { m_permCP = _pos; }

		/** \brief A "this" pointer*/
		std::weak_ptr<PhysicsEventUser> m_self;

		protected:
		/** \brief Marks whether this has collided or not*/
		bool m_collided = false;

		/** \brief Floored is an optimisation. If an object is "floored" it is sitting on a flat surface and no further gravity or collision checks need to happen.*/
		bool m_floored = false;

		/** \brief  A "permanent" contact-point that stores where an object should remain once stationary (ie: stacked on top of another object)*/
		glm::vec3 m_permCP; 

		/** \brief This is called when the object should resolve the collisions it has stored*/
		void handleCollisions();

		/** \brief Checks to see if a collision is inside the list of collisions this object holds*/
		bool isInColList(std::vector<std::shared_ptr<Collision>> _list, std::shared_ptr<Collision> _col);

		/** \brief The last collision the object encountered*/
		std::vector<std::shared_ptr<Collision>> m_lastCol;

		/** \brief A list of all collisions that this object has accumulated*/
		std::vector<std::shared_ptr<Collision>> m_collisions;
	};

	/** \brief A Physics Object checks for collisions each frame automatically, obeys gravity and carries a velocity*/
	/** It currently does not support rotated colliders or torque, but is the standard Physics object which most physical objects should use */
	class PhysicsObject : public PhysicsEventUser
	{
		

		public:
		/** \brief Update, checks for collisions*/
		void onTick();

		/** \brief Initialise the object with a mass value and a bounciness value*/
		void onInitialise(float _mass, float _bounciness);
		PhysicsObject();

		/** \brief Add force to an object*/
		void addForce(glm::vec3 _force) { m_force += _force; }

		/** \brief Clear all forces on an object*/
		void clearForces() { m_force = glm::vec3(0.0f, 0.0f, 0.0f); }

		/** \brief Performs updates velocity based on deltatime and force on the object and returns a delta position*/
		glm::vec3 rungeKutta2(float _deltaTs, float _mass);

		/** \brief Set the force on the object to a value*/
		void setForce(const glm::vec3 _force) { m_force = _force; }

		/** \brief Get the velocity of the object*/
		glm::vec3 getVelocity() { return m_velocity; }

		/** \brief Sets the velocity to a value*/
		void setVelocity(glm::vec3 _vel);

		/** \brief Add to the velocity of the object*/
		void addVelocity(glm::vec3 _vel) { m_velocity += _vel; }

		/** \brief Get the bounciness value of the object*/
		float getBounciness() { return m_bounciness; }

		/** \brief Set the bounciness of the object*/
		void setBounciness(float _amt) { m_bounciness = _amt; }

		/** \brief Gets the mass of the object*/
		float getMass() { return m_mass; }

		/** \brief Sets the mass of the object*/
		void setMass(float _mass) { m_mass = _mass; }

		/** \brief Gets the current force of the object*/
		const glm::vec3 getForce() const { return m_force; }

		/** \brief A "this" pointer*/
		std::weak_ptr<PhysicsObject> m_self;

		protected:
		
		
		/** \brief The mass of the object*/
		float m_mass = 1.0f;	

		/** \brief The bounciness value of the object*/
		float m_bounciness = 0.0f;

		/** \brief The velocity of the object*/
		glm::vec3 m_velocity = glm::vec3(0.0f);

		/** \brief The force of the object*/
		glm::vec3 m_force = glm::vec3(0.0f);
	};

	/** \brief An Advanced Physics object which can rotate, have torque and an angular velocity*/
	/** Currently only compatible for spheres */
	class AdvPhysicsObject : public PhysicsObject
	{
		public:
		/** \brief Update*/
		void onTick();

		/** \brief Initialise with a mass and bounciness value*/
		void onInitialise(float _mass, float _bounciness);
		AdvPhysicsObject() {};

		/** \brief Returns the rotation matrix of the object*/
		glm::mat3 getRotMat() { return m_rotMat; }
		
		/** \brief Updates the rotation of the object using its angular velocities*/
		void updateRotations(float _dTs, bool collided);

		/** \brief Sets the angular velocity of the object*/
		void setAvel(glm::vec3 _aVel) { m_aVel = _aVel; }

		/** \brief Gets the angular velocity of the object*/
		glm::vec3 getAvel() { return m_aVel; }

		/** \brief Assigns the rotation matrix of the object*/
		void setRotMat(glm::mat3 _rMat) { m_rotMat = _rMat; }

		/** \brief Calculates the inverse tensor of the object*/
		glm::mat3 calcInvTensor();

		/** \brief Assigns the inverse tensor of the object*/
		void setInvTensor(glm::mat3 _matrix) { m_invInertiaTensor = _matrix; }

		/** \brief Gets the Angular Momentum of the object*/
		glm::vec3 getAmom() { return m_aMom; }

		/** \brief Sets the angular momentum of the object*/
		void setAmom(glm::vec3 _mom) { m_aMom = _mom; }

		/** \brief Adds torque to the object*/
		void addTorque(glm::vec3 _torque) { m_torque += _torque; }

		/** \brief Removes any torque from the object*/
		void clearTorque() { m_torque = glm::vec3(0.0f, 0.0f, 0.0f); }

		/** \brief Return the rotation quaternion*/
		glm::quat getRotQuat() { return m_rotQuat; }

		/** \brief A "this" pointer*/
		std::weak_ptr<AdvPhysicsObject> m_self;

		private:
		//Angular Motion
		/** \brief The torque force acting on the object this frame*/
		glm::vec3 m_torque = glm::vec3(0.0f);

		/** \brief  Angular Velocity*/
		glm::vec3 m_aVel = glm::vec3(0.0f); 
		/** \brief  Angular momentum*/
		glm::vec3 m_aMom = glm::vec3(0.0f); 

		/** \brief Angular acceleration	*/
		glm::vec3 m_aAcc = glm::vec3(0.0f); 	

		/** \brief  Inverse of the inertia tensor*/
		glm::mat3 m_invInertiaTensor;

		/** \brief The inverse of the Body inertia tensor*/
		glm::mat3 m_invBodyInertiaTensor;

		/** \brief The matrix that represents the rotation of the object*/
		glm::mat3 m_rotMat;

		/** \brief The quaternion which represents the rotation of the object*/
		glm::quat m_rotQuat;
		
	};
	
	/** \brief A component for firing rays against geometry		*/
	class RayCaster : public Component
	{
		public:
		/** \brief Call this to Raycast against a vector of Entities	*/		
		/** You may want to make use of the Core getAllEntitites function as the argument for the vector of Entities. The other arguments are raydirection, ray origin and a tag for the ray */
		std::shared_ptr<RayCollision> raycastAgainst(std::vector<std::shared_ptr<Entity>> _obj, glm::vec3 _rayDir, glm::vec3 _origin, std::string _rayTag);

		/** \brief A "This" pointer*/
		std::weak_ptr<RayCaster> m_self;

		private:
		/** \brief Checks a ray against a single triangle*/
		int intersect_triangle3(double orig[3], double dir[3], double vert0[3], double vert1[3], double vert2[3], double *t, double *u, double *v);
		//Reference Tomas Moller
		//T = distance to the triangle
		//U and V are two points inside the triangle
	};
}
#endif