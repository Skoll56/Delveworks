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
		bool m_trigger = false;
	private:
		
		float m_friction = 0.25f;		
	};

	class SphereCollider : public Collider
	{
	public:
		SphereCollider(float _radius);
		SphereCollider() {};
		float getRadius() { return transform()->getScale().y; }
	};

	class PlaneCollider : public Collider
	{
	public:		
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
		//MeshCollider(std::string _obj, glm::vec3 _scale, glm::vec3 _size);
		//MeshCollider(glm::vec3 _scale);
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
		bool m_semiMesh = false;
		bool m_easyCollide = false;
		std::string m_lastMesh = "NA";
		std::vector<int> m_colTri;
		std::vector<int> m_myTri;
		bool m_collidedBefore = false;
		float m_precision = 0.01f;
	};
}

#endif