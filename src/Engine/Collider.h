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
	class Collider : public Component
	{
	public:
		void setFriction(float _friction) { m_friction = _friction; }
		float getFriction() { return m_friction; }
		float getMass() { return m_mass; }
	private:
		bool m_trigger;
		float m_friction;
		float m_mass;
	};

	class SphereCollider : public Collider
	{
	public:
		SphereCollider(float _radius);
		SphereCollider() {};
		float getRadius() { return m_radius; }


	private:
		float m_radius;

	};

	class PlaneCollider : public Collider
	{
	public:
		PlaneCollider(glm::vec3 _scale, glm::vec3 _normal);
		PlaneCollider() {};
		glm::vec3 getNorm() { return m_normal; }
		void setNorm(glm::vec3 _norm) { m_normal = _norm; }

	private:
		glm::vec3 m_normal;
	};


	class BoxCollider : public Collider
	{
	public:
		BoxCollider(glm::vec3 _scale);
		BoxCollider() {};

	private:
		std::vector<PlaneCollider> m_face;
	};


	class MeshCollider : public Collider
	{
	public:
		MeshCollider(std::string _obj, glm::vec3 _scale, glm::vec3 _size);
		MeshCollider(glm::vec3 _scale);
		MeshCollider() {};
		void setEasy(bool _val) { m_easyCollide = _val; }
		bool isEasy() { return m_easyCollide; }
		void setColBefore(bool _val) { m_collidedBefore = _val; }
		bool colBefore() { return m_collidedBefore; }
		std::string getLastCol() { return m_lastMesh; }
		void setLastCol(std::string _tag) { m_lastMesh = _tag; }
		float getPrecision() { return m_precision; }
		void setPrecision(float _prec) { m_precision = _prec; }
		std::vector<int> getColTri() { return m_colTri; }
		std::vector<int> getMyTri() { return m_myTri; }
		void setMyTri(std::vector<int> _myTri) { m_myTri = _myTri; }
		void setColTri(std::vector<int> _colTri) { m_colTri = _colTri; }


	private:
		bool m_semiMesh;
		bool m_easyCollide;
		std::string m_lastMesh;
		std::vector<int> m_colTri;
		std::vector<int> m_myTri;
		bool m_collidedBefore;
		float m_precision;
	};
}

#endif