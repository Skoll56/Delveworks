#ifndef _COLLIDER_H_
#define _COLLIDER_H_

#include "Component.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Engine
{	
	struct RayCollision;

	/** \brief  The Collider Base Class.*/
	/** This is the base class for any Collider. Colliders work with PhysicsEventUser objects to generate Physics events */
	class Collider : public Component
	{
	public:		
		void setFriction(float _friction) { m_friction = _friction; }
		float getFriction() { return m_friction; }

		/** \brief A Trigger isn't a solid object, PhysicsObject will phase through it but a collision event still gets generated*/
		bool m_trigger = false;

		/** \brief A "this" pointer*/
		std::weak_ptr<Collider> m_self;

	protected:		
		/** \brief Any friction this object has.*/
		float m_friction = 0.25f;	
		Collider() {};
	};

	/** \brief Spheres*/
	class SphereCollider : public Collider
	{
	public:		
		SphereCollider() {};
		/** \brief A "This" pointer*/
		std::weak_ptr<SphereCollider> m_self;
		float getRadius() { return transform()->getScale().y; }
	};
	 /** \brief Planes*/
	class PlaneCollider : public Collider
	{
	public:		
		PlaneCollider() {};
		glm::vec3 getNorm() { return m_normal; } //TODO: This is dumb, just use the 'Up' vector.
		void setNorm(glm::vec3 _norm) { m_normal = _norm; }

		/** \brief A "This" pointer*/
		std::weak_ptr<PlaneCollider> m_self;

	private:
		/** \brief This is the "Normal" of the plane.*/
		/** A plane that is lying flat will have a normal of either 0,1,0 or 0,-1,0 (Up or down) */
		glm::vec3 m_normal;
	};

	/** \brief Boxes*/
	class BoxCollider : public Collider
	{
	public:		
		BoxCollider() {};
		/** \brief A "This" pointer*/
		std::weak_ptr<BoxCollider> m_self;

	private: 
		
	};

	/** \brief Meshes
	/** This is the most expensive and least reliable collider class. Use only when necessary. */
	class MeshCollider : public Collider
	{
		friend class PhysicsObject;
		friend class AdvPhysicsObject;
		friend class PhysicsEventUser;

	public:		
		MeshCollider() {};
		/** \brief Sets whether to easy collide or not*/
		void setEasy(bool _val) { m_easyCollide = _val; }

		/** \brief Returns whether the mesh will easy collide or not*/
		bool isEasy() { return m_easyCollide; }		
		
		/** \brief Returns the Entity tag of the mesh we most recently collided with*/
		std::string getLastCol() { return m_lastMesh; }
		
		/** \brief Returns how 'precise' the collision reaction is.*/
		float getPrecision() { return m_precision; }
		/** \brief Sets how 'precise' the collision reaction is. It is unlikely you will need to edit the default value.*/
		void setPrecision(float _prec) { m_precision = _prec; }

		/** \brief A "This" pointer*/
		std::weak_ptr<MeshCollider> m_self;

	private:
		/** \brief An "Easy Collide" mesh loses much accuracy in return for performance. */
		bool m_easyCollide = false; //TODO: This should be a box from the furthest points

		/** \brief The tag of the last mesh that we collided with*/
		std::string m_lastMesh = "NA"; //TODO: Can't this just be an Entity reference? Or a collider reference?

		/** \brief A reference to the last triangle I collided with*/
		std::vector<int> m_colTri;

		/** \brief A reference to my last triangle I used to collide with something*/
		std::vector<int> m_myTri;

		/** \brief Marks if this Mesh has ever collided*/
		bool m_collidedBefore = false;
		void setColBefore(bool _val) { m_collidedBefore = _val; }
		bool colBefore() { return m_collidedBefore; }

		/** \brief How precise the "Nudge" step is when moving an object out of this mesh. Lower values are more precise.*/
		float m_precision = 0.01f;
		

		/** \brief Sets the tag of the object most recently collided with*/
		void setLastCol(std::string _tag) { m_lastMesh = _tag; }

		/** \brief Gets the colTri*/
		std::vector<int> getColTri() { return m_colTri; }

		/** \brief Gets this object's colTri*/
		std::vector<int> getMyTri() { return m_myTri; }

		/** \brief Sets this object's coltri*/
		void setMyTri(std::vector<int> _myTri) { m_myTri = _myTri; }

		/** \brief Sets the colTri*/
		void setColTri(std::vector<int> _colTri) { m_colTri = _colTri; }
	};
}

#endif