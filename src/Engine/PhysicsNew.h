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

	class PhysicsEventUser : public Component
	{
	public:
		void onTick();
		
		bool checkForCollisions(float _dT, std::vector<std::shared_ptr<Entity>> _other, glm::vec3 _c1);
		
		void resetCollisions();

		void setCollided(bool _val) { m_collided = _val; }
		bool collided() { return m_collided; }

		void onCollision(Collision* _col);

		void setFloored(bool _val) { m_floored = _val; }
		bool isFloored() { return m_floored; }


		bool boxToPlane(std::shared_ptr<BoxCollider> _my, std::shared_ptr<PlaneCollider> _other, glm::vec3 _c1);

		int coplanar_tri_tri(float N[3], float V0[3], float V1[3], float V2[3],
			float U0[3], float U1[3], float U2[3]);

		int NoDivTriTriIsect(float V0[3], float V1[3], float V2[3],
			float U0[3], float U1[3], float U2[3]);

		int triBoxOverlap(float boxcenter[3],
			float boxhalfsize[3], float triverts[3][3]);

		int planeBoxOverlap(float normal[3], float d, float maxbox[3]);

		float distanceToPlane(glm::vec3 &n, glm::vec3 &p, glm::vec3 &q);

		bool boxToBox(std::shared_ptr<BoxCollider> _my, std::shared_ptr<BoxCollider> _other, glm::vec3 _c1);

		bool meshToMesh(std::shared_ptr<MeshCollider> _my, std::shared_ptr<Collider> _other, glm::vec3 _c1);

		bool sphereToPlane(std::shared_ptr<SphereCollider> _my, std::shared_ptr<PlaneCollider> _other, glm::vec3 _c1);

		bool sphereToBox(std::shared_ptr<SphereCollider> _my, std::shared_ptr<BoxCollider> _other, glm::vec3 _c1);
		bool sphereToSphere(std::shared_ptr<SphereCollider> _my, std::shared_ptr<SphereCollider> _other);
		bool boxToSphere(std::shared_ptr<BoxCollider> _my, std::shared_ptr<SphereCollider> _other, glm::vec3 _c1);

		bool sphereBoxOverlap(std::shared_ptr<SphereCollider> _sphere, std::shared_ptr<BoxCollider> _box);

		
		glm::vec3 roundNormals(glm::vec3 _n);
		glm::vec3 getPermCP() { return m_permCP; }
		void setPermCP(glm::vec3 _pos) { m_permCP = _pos; }


	protected:
		bool m_collided;
		bool m_floored;
		glm::vec3 m_permCP; // A "permanent" contact-point that stores where an object should remain once stationary (ie: stacked on top of another object)
		std::vector<Collision*> m_collisions;
	};

	class PhysicsObject : public PhysicsEventUser
	{
	public:
		void onTick();

		PhysicsObject(float _mass, float _bounciness, Collider* _col);
		PhysicsObject();

		void handleCollisions();
		void addForce(glm::vec3 _force) { m_force += _force; }
		void clearForces() { m_force = glm::vec3(0.0f, 0.0f, 0.0f); }

		glm::vec3 rungeKutta2(float _deltaTs, float _mass);

		void setForce(const glm::vec3 _force) { m_force = _force; }
		glm::vec3 getVelocity() { return m_velocity; }
		void setVelocity(glm::vec3 _vel);
		void addVelocity(glm::vec3 _vel) { m_velocity += _vel; }

		float getBounciness() { return m_bounciness; }
		void setBounciness(float _amt) { m_bounciness = _amt; }

		float getMass() { return m_mass; }
		void setMass(float _mass) { m_mass = _mass; }

		const glm::vec3 getForce() const { return m_force; }

		

		//Mesh-Mesh specific variables needed for optimisation. I'll probably put this somewhere tidier later.
		void setColBefore(bool _val) { m_collidedBefore = _val; }
		bool colBefore() { return m_collidedBefore; }
		std::vector<int> getColTri() { return m_colTri; }
		std::vector<int> getMyTri() { return m_myTri; }
		void setMyTri(std::vector<int> _myTri) { m_myTri = _myTri; }
		void setColTri(std::vector<int> _colTri) { m_colTri = _colTri; }
		
		std::string getLastCol() { return m_lastMesh; }
		void setLastCol(std::string _tag) { m_lastMesh = _tag; }		
		void setCollider(bool _val) { m_collider = _val; }
		bool isCollider() { return m_collider; }

	protected:
		
		float m_mass;
		bool m_collider;
		float m_bounciness;
		glm::vec3 m_velocity;
		glm::vec3 m_force;
		float m_precision;
		
		float m_resist;
		std::string m_lastMesh;
		std::vector<int> m_colTri;
		std::vector<int> m_myTri;
		bool m_collidedBefore;
		
	};

	class AdvPhysicsObject : public PhysicsObject
	{
	public:
		void onTick();
		AdvPhysicsObject(float _mass, float _bounciness, Collider* _col);
		AdvPhysicsObject() {};
		glm::mat3 getRotMat() { return m_rotMat; }
		void updateRotations(float _dTs, bool collided);
		void setAvel(glm::vec3 _aVel) { m_aVel = _aVel; }
		glm::vec3 getAvel() { return m_aVel; }
		void setRotMat(glm::mat3 _rMat) { m_rotMat = _rMat; }
		glm::mat3 calcInvTensor();
		void setInvTensor(glm::mat3 _matrix) { m_invInertiaTensor = _matrix; }
		glm::vec3 getAmom() { return m_aMom; }
		void setAmom(glm::vec3 _mom) { m_aMom = _mom; }
		void addTorque(glm::vec3 _torque) { m_torque += _torque; }
		void clearTorque() { m_torque = glm::vec3(0.0f, 0.0f, 0.0f); }
		glm::quat getRotQuat() { return m_rotQuat; }

	private:
		//Angular Motion
		glm::vec3 m_torque;
		glm::vec3 m_aVel; // Angular Velocity
		glm::vec3 m_aMom; // Angular momentum
		glm::vec3 m_aAcc; //Angular acceleration		
		glm::mat3 m_invInertiaTensor; // Inverse of the body inertia tensor
		glm::mat3 m_invBodyInertiaTensor;
		glm::mat3 m_rotMat;
		glm::quat m_rotQuat;
		glm::vec3 m_orient;		
	};
		
	class RayCaster : public Component
	{
		public:
		//Reference Tomas Moller
		int intersect_triangle3(double orig[3], double dir[3], double vert0[3], double vert1[3], double vert2[3], double *t, double *u, double *v);
		//T = distance to the triangle
		//U and V are two points inside the triangle
		RayCollision* rayToTri(std::vector<std::shared_ptr<Entity>> _obj, glm::vec3 _rayDir, glm::vec3 _origin, std::string _rayTag);
	};
}
#endif