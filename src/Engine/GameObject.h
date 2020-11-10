#ifndef __GAME_OBJECT__
#define __GAME_OBJECT__

#include <vector>
#include <string>
#include "Components.h"
#include "Shader.h"

namespace Engine
{
	class GameObject
	{
	public:

		GameObject(glm::vec3 _startPos);
		GameObject();
		~GameObject();

		std::string getTag() { return m_tag; }
		void setTag(std::string _tag) { m_tag = _tag; }

		void setPosition(float _posX, float _posY, float _posZ) { m_position.x = _posX; m_position.y = _posY; m_position.z = _posZ; }
		void setPosition(glm::vec3 _value) { m_position = _value; }

		glm::vec3 getPosition() { return m_position; }

		bool isActive() { return m_active; };
		void setActive(bool _status) { m_active = _status; }

		void setShader(std::shared_ptr<Shader> _shader) { m_shader = _shader; }
		std::shared_ptr<Shader> getShader() { return m_shader; }


		//Component functions
		void addPlaneShape(std::string _texName, glm::vec3 _scale, glm::vec3 _normal);
		void addBoxShape(std::string _texName, glm::vec3 _scale);
		void addSphereShape(std::string _texName, float _radius);
		void addMeshShape(std::string _texName, std::string _obj, glm::vec3 _scale, glm::vec3 _size, std::string _tag);
		void addMeshShape(std::string _texName, glm::vec3 _scale, std::string _tag);
		void deleteShape();
		void addStats(float _hp, float _dmg, float _speed);

		bool hasStats() { return m_stats; }
		void addPhysics(std::string _tag, float _mass, float _bounciness); // Adds a rigidbody
		void addAdvancedPhysics(std::string _shape, glm::vec3 _scale, float _mass); //Adds rotational-physics
		void removePhysics();
		bool isPhysics() { return m_physics; } // A boolean that holds if it is a physics object or not
		bool isAdvancedPhysics() { return m_advancedPhys; }
		std::string getShape() { return m_shape; } // shape (ie: box, sphere, etc)
		void setShapeString(std::string _val) { m_shape = _val; }
		void setPhysBool(bool _val) { m_physics = _val; }
		void setAdPhysBool(bool _val) { m_advancedPhys = _val; }
		void setRoom(int _num) { m_roomNum = _num; }
		int getRoomNum() { return m_roomNum; }
		void setDelete(bool _val) { m_delete = _val; }
		bool getDelete() { return m_delete; }
		void setStats(bool _val) { m_stats = _val; }

		//Component variables
		PhysicsObj *m_rb;
		Plane *m_plane;
		Sphere *m_sphere;
		Box *m_box;
		Mesh *m_mesh;
		Shape *m_shapeComp;
		AdvancedPhysics *m_Phy;
		Stats *m_charSheet;

	protected:
		std::shared_ptr<Shader> m_shader;
		glm::vec3 m_position;
		std::string m_tag;
		bool m_active;
		int m_roomNum;
		bool m_delete;
		//Component Variables
		std::string m_shape;
		bool m_physics;
		bool m_advancedPhys;
		bool m_stats;
	};
}

#endif
