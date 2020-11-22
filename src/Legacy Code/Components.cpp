
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Texture.h"
#include "Collision.h"
#include "Components.h"
#include "Utility.h"
#include <iostream>

namespace Engine
{
	AdvancedPhysics::AdvancedPhysics(std::string _shape, glm::vec3 _scale, float _mass)
	{
		m_torque = glm::vec3(0.0f, 0.0f, 0.0f);
		m_aMom = glm::vec3(0.0f, 0.0f, 0.0f);
		m_rotMat = glm::mat3(1.0f);
		m_rotQuat = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
		m_rotQuat = glm::rotate(m_rotQuat, 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));


		m_orient = glm::vec3(0.0f, 0.0f, 0.0f);

		//A sphere has a body inertia tensor of 2/5 * mass * r^2

		if (_shape == "sphere")
		{
			m_invBodyInertiaTensor = glm::mat3((2.0f / 5.0f) * _mass * pow(_scale.y, 2));
		}


		else if (_shape == "box")
		{
			glm::vec3 sizeS = _scale / 2.0f;
			sizeS *= sizeS;
			float tensor = 1.0f / 12.0f;
			float x = (sizeS.y + sizeS.z) * _mass * tensor;
			float y = (sizeS.x + sizeS.z) * _mass * tensor;
			float z = (sizeS.x + sizeS.y) * _mass * tensor;
			m_invBodyInertiaTensor =
				glm::mat3(
					x, 0, 0,
					0, y, 0,
					0, 0, z);
		}

		m_invBodyInertiaTensor = glm::inverse(m_invBodyInertiaTensor);
		m_invInertiaTensor = calcInvTensor();
		m_aVel = m_invInertiaTensor * m_aMom;
	}

	PhysicsObj::PhysicsObj(float _mass, float _bounciness)
	{
		m_mass = _mass;
		m_bounciness = _bounciness;
		m_force = glm::vec3(0.0f, 0.0f, 0.0f);
		m_precision = 0.01f;
		m_collidedBefore = false;
		m_lastMesh = "NA";
		m_floored = false;
	}

	PhysicsObj::PhysicsObj()
	{
		m_mass = 10.0f;
		m_bounciness = 0.0f;
		m_precision = 0.1f;
		m_collidedBefore = false;
		m_lastMesh = "NA";
	}

	glm::vec3 PhysicsObj::rungeKutta2(float _deltaTs, float _mass)
	{
		glm::vec3 a = m_force / _mass;
		glm::vec3 k1 = _deltaTs * a; // Step one, ordinary Euler calculation at t= 0
		a = (m_force + k1 / 2.0f) / _mass;
		glm::vec3 k2 = _deltaTs * a;  // Evaluate again at t= 0.5 before adjusting the velocity
		m_velocity += k2;
		return (m_velocity * _deltaTs);
	}

	void PhysicsObj::setVelocity(glm::vec3 _vel)
	{
		m_velocity = _vel;
	}


	glm::mat3 AdvancedPhysics::calcInvTensor()
	{
		return (m_rotMat * m_invBodyInertiaTensor * glm::transpose(m_rotMat));
	}

	void AdvancedPhysics::updateRotations(float _dTs, bool _collided)
	{
		m_aMom += m_torque * _dTs;
		if (_collided)
		{
			m_aMom.x *= 0.98f;//Reference Alex
			m_aMom.y *= 0.98f;
			m_aMom.z *= 0.98f;
		}
		setInvTensor(calcInvTensor());
		m_aVel = m_invInertiaTensor * m_aMom;

		glm::mat3 skew(
			0.0f, -m_aVel.z, m_aVel.y,
			m_aVel.z, 0.0f, -m_aVel.x,
			-m_aVel.y, m_aVel.x, 0.0f
		);
		m_rotMat += (skew * m_rotMat* _dTs);

	}

	void PhysicsObj::resetCollisions()
	{
		m_collisions.clear();
	}

	Box::Box(std::string _texName, glm::vec3 _scale)
	{
		m_vBOp = new VertexBuffer();
		m_vBOt = new VertexBuffer();
		m_vAO = new VertexArray();
		m_tex = new Texture();
		m_rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		m_size = glm::vec3(1.0f, 1.0f, 1.0f);
		m_scale = _scale;
		m_solid = true;
		m_tex->load(_texName);
		m_vAO->loadObj("1b1cube.obj");
		m_friction = 1.0f;
		m_shininess = 1;
	}

	Plane::Plane(std::string _texName, glm::vec3 _scale, glm::vec3 _normal)
	{
		m_vBOp = new VertexBuffer();
		m_vBOt = new VertexBuffer();
		m_vAO = new VertexArray();
		m_tex = new Texture();
		m_rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		m_scale = _scale;

		m_size = glm::vec3(1.0f, 1.0f, 1.0f);
		m_mass = INFINITY;
		m_solid = true;
		m_normal = _normal;
		if (_scale.y != 0.0f)
		{
			m_vAO->loadObj("1b1cube.obj");
		}
		else
		{
			m_vAO->loadObj("1b1plane.obj");
		}
		m_tex->load(_texName);
		m_friction = 5.0f;
		m_shininess = 1;
	}

	Sphere::Sphere(std::string _texName, float _radius)
	{
		m_vBOp = new VertexBuffer();
		m_vBOt = new VertexBuffer();
		m_vAO = new VertexArray();
		m_tex = new Texture();
		m_rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		m_scale = glm::vec3(_radius, _radius, _radius);
		//m_tex->addTexture(_texName);
		m_size = glm::vec3(1.0f, 1.0f, 1.0f);
		m_mass = INFINITY;
		m_solid = true;
		m_radius = _radius;
		m_vAO->loadObj("1b1sphere.obj");
		m_tex->load(_texName);
		m_friction = 1.0f;
		m_shininess = 1;
	}

	Mesh::Mesh(std::string _texName, std::string _obj, glm::vec3 _scale, glm::vec3 _size)
	{
		m_easyCollide = false;
		m_vBOp = new VertexBuffer();
		m_vBOt = new VertexBuffer();
		m_vAO = new VertexArray();
		m_tex = new Texture();
		m_rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		m_scale = _scale;
		m_size = _size;
		m_mass = INFINITY;
		m_solid = true;
		std::cout << "Got this far" << std::endl;
		m_vAO->loadObj(_obj);
		std::cout << "And here" << std::endl;
		m_tex->load(_texName);
		std::cout << "And also here" << std::endl;
		m_friction = 1.0f;
		m_shininess = 1;
		m_semiMesh = false;
	}

	Mesh::Mesh(std::string _texName, glm::vec3 _scale)
	{

		m_easyCollide = false;

		m_vBOp = new VertexBuffer();
		m_vAO = new VertexArray();

		m_vBOp->add(glm::vec3(0.0f, 1.0f, 0.0f));
		m_vBOp->add(glm::vec3(-1.0f, -1.0f, 0.0f));
		m_vBOp->add(glm::vec3(1.0f, -1.0f, 0.0f));
		m_vAO->setBuffer("in_Position", m_vBOp);

		m_vBOt = new VertexBuffer();
		m_tex = new Texture();
		m_vBOt->add(glm::vec2(0.5f, 0.0f));
		m_vBOt->add(glm::vec2(0.0f, 1.0f));
		m_vBOt->add(glm::vec2(1.0f, 1.0f));
		m_vAO->setBuffer("in_TexCoord", m_vBOt);



		m_rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		m_scale = _scale;
		m_mass = INFINITY;
		m_solid = true;

		m_tex->load(_texName);
		m_friction = 1.0f;
		m_shininess = 1;
		m_semiMesh = false;
	}
}
