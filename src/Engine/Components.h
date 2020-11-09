#ifndef _COMPONENTS_H_
#define _COMPONENTS_H_
#include <vector>
#include <string>
#include <glm/glm.hpp>

namespace Engine
{

	class GameObject;
	class Collision;
	class VertexArray;
	class VertexBuffer;
	class Texture;


	class OldComponent //All the components used in the engine are currently stored here
	{

	};

	class Stats : public OldComponent
	{
	public:
		Stats(float _hp, float _dmg, float _speed)
		{
			m_HP = _hp;
			m_DMG = _dmg;
			m_speed = _speed;
			m_direction = glm::vec3(0.0f, 0.0f, 0.0f);
			m_hornet = false;
			m_zter = false;
			m_player = false;
		}
		float getHP() { return m_HP; }
		float getDMG() { return m_DMG; }
		float getSpeed() { return m_speed; }
		glm::vec3 getDir() { return m_direction; }

		void setHP(float _hp) { m_HP = _hp; }
		void addHP(float _amt) { m_HP += _amt; }
		void setSpeed(float _speed) { m_speed = _speed; }
		void setDir(glm::vec3 _dir) { m_direction = _dir; }
		void setDMG(float _dmg) { m_DMG = _dmg; }
		bool isPlayer() { return m_player; }
		bool isHornet() { return m_hornet; }
		bool isZter() { return m_zter; }
		void setZter() { m_zter = true; }
		void setHornet() { m_hornet = true; }

	private:
		glm::vec3 m_direction;
		float m_HP;
		float m_DMG;
		float m_speed;

		//Checks what type of entity they are
		bool m_player;
		bool m_hornet;
		bool m_zter;
	};

	class Shape : public OldComponent
	{
	public:
		VertexArray * m_vAO; // The shape's mesh/obj
		VertexBuffer *m_vBOt; // The vertex buffer for texture co-ordinates
		VertexBuffer *m_vBOp; // The vertex buffer for object co-ordinates
		Texture *m_tex;

		bool isSolid() { return m_solid; }
		void setSolid(bool _solid) { m_solid = _solid; }
		glm::vec3 getSize() { return m_size; }

		void setScale(float _sX, float _sY, float _sZ) { m_scale.x = _sX; m_scale.y = _sY; m_scale.z = _sZ; }
		glm::vec3 getScale() { return m_scale; }
		float getMass() { return m_mass; }
		void setShine(int _shininess) { m_shininess = _shininess; }
		int getShine() { return m_shininess; }
		void rotate(glm::vec3 _change) { m_rotation += _change; }
		void setRotation(float _rotX, float _rotY, float _rotZ) { m_rotation.x = _rotX; m_rotation.y = _rotY; m_rotation.z = _rotZ; }
		void setRotation(glm::vec3 _rot) { m_rotation = _rot; }
		glm::vec3 getRot() { return m_rotation; }
		glm::mat4 getModel() { return m_model; }
		void setModel(glm::mat4 _model) { m_model = _model; }
		void setFriction(float _friction) { m_friction = _friction; }
		float getFriction() { return m_friction; }


	protected:

		glm::mat4 m_model;
		bool m_solid;
		float m_mass;
		glm::vec3 m_rotation;
		glm::vec3 m_size; // The size of the obj in generic units (ie: box objs are 1/1/1 units)
		glm::vec3 m_scale;
		float m_friction;
		int m_shininess;
	};


	class Sphere : public Shape
	{
	public:
		Sphere(std::string _texName, float _radius);
		Sphere() {};
		float getRadius() { return m_radius; }
		void setScale(float _radius) { m_scale.x = _radius; m_scale.y = _radius; m_scale.z = _radius; }

	private:
		float m_radius;

	};



	class Plane : public Shape
	{
	public:
		Plane(std::string _texName, glm::vec3 _scale, glm::vec3 _normal);
		Plane() {};
		glm::vec3 getNorm() { return m_normal; }
		void setNorm(glm::vec3 _norm) { m_normal = _norm; }

	private:
		glm::vec3 m_normal;
	};


	class Box : public Shape
	{
	public:
		Box(std::string _texName, glm::vec3 _scale);
		Box() {};

	private:
		std::vector<Plane> m_face;
	};


	class Mesh : public Shape
	{
	public:
		Mesh(std::string _texName, std::string _obj, glm::vec3 _scale, glm::vec3 _size);
		Mesh(std::string _texName, glm::vec3 _scale);
		Mesh() {};
		void setEasy(bool _val) { m_easyCollide = _val; }
		bool isEasy() { return m_easyCollide; }
	private:
		bool m_semiMesh;
		bool m_easyCollide;
	};


	class PhysicsObj : public OldComponent
	{
	public:
		PhysicsObj(float _mass, float _bounciness);
		PhysicsObj();

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

		glm::vec3 getPermCP() { return m_permCP; }
		void setPermCP(glm::vec3 _pos) { m_permCP = _pos; }

		std::vector<Collision*> m_collisions;

		void setCollided(bool _val) { m_collided = _val; }
		bool collided() { return m_collided; }
		void resetCollisions();

		//Mesh-Mesh specific variables needed for optimisation. I'll probably put this somewhere tidier later.
		void setColBefore(bool _val) { m_collidedBefore = _val; }
		bool colBefore() { return m_collidedBefore; }
		std::vector<int> getColTri() { return m_colTri; }
		std::vector<int> getMyTri() { return m_myTri; }
		void setMyTri(std::vector<int> _myTri) { m_myTri = _myTri; }
		void setColTri(std::vector<int> _colTri) { m_colTri = _colTri; }
		float getPrecision() { return m_precision; }
		void setPrecision(float _prec) { m_precision = _prec; }
		std::string getLastCol() { return m_lastMesh; }
		void setLastCol(std::string _tag) { m_lastMesh = _tag; }
		void setFloored(bool _val) { m_floored = _val; }
		bool isFloored() { return m_floored; }
		void setCollider(bool _val) { m_collider = _val; }
		bool isCollider() { return m_collider; }

	private:
		glm::vec3 m_permCP; // A "permanent" contact-point that stores where an object should remain once stationary (ie: stacked on top of another object)
		float m_mass;
		bool m_collider;
		float m_bounciness;
		glm::vec3 m_velocity;
		glm::vec3 m_force;
		float m_precision;
		bool m_collided;
		float m_resist;
		std::string m_lastMesh;
		std::vector<int> m_colTri;
		std::vector<int> m_myTri;
		bool m_collidedBefore;
		bool m_floored;
	};

	class AdvancedPhysics
	{
	public:
		AdvancedPhysics(std::string _shape, glm::vec3 _scale, float _mass);
		AdvancedPhysics() {};
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
	private:
		//Angular Motion
		glm::vec3 m_torque;
		glm::vec3 m_aVel; // Angular Velocity
		glm::vec3 m_aMom; // Angular momentum
		glm::vec3 m_aAcc;
		glm::mat3 m_invInertiaTensor; // Inverse of the body inertia tensor
		glm::mat3 m_invBodyInertiaTensor;
		glm::mat3 m_rotMat;
		glm::vec3 m_orient;
	};
}

#endif