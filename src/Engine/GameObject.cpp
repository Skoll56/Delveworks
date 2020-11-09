#include "GameObject.h"
namespace Engine
{

	/*! \brief Brief description.
	*  GameObject class contains a list of possible components.
	*
	*/


	GameObject::GameObject(glm::vec3 _startPos)
	{
		m_position = _startPos;
		m_tag = "untagged";
		m_shape = "NO SHAPE";
		m_active = true;
		m_physics = false;
		m_advancedPhys = false;
		m_stats = false;
		m_delete = false;
	}

	GameObject::GameObject()
	{
		m_tag = "untagged";
		m_shape = "NO SHAPE";
		m_active = true;
		m_physics = false;
		m_advancedPhys = false;
		m_stats = false;
		m_delete = false;		
	}


	GameObject::~GameObject()
	{
		// Do any clean up here
	}

	void GameObject::addPlaneShape(std::string _texName, glm::vec3 _scale, glm::vec3 _normal)
	{
		m_shape = "plane";
		m_plane = new Plane(_texName, _scale, _normal);
		m_shapeComp = m_plane;
		
		
	}

	void GameObject::addBoxShape(std::string _texName, glm::vec3 _scale)
	{
		m_shape = "box";
		m_box = new Box(_texName, _scale);
		m_shapeComp = m_box;
		
	}

	void GameObject::addSphereShape(std::string _texName, float _radius)
	{
		m_shape = "sphere";
		m_sphere = new Sphere(_texName, _radius);
		m_shapeComp = m_sphere;
		
	}

	void GameObject::addMeshShape(std::string _texName, std::string _obj, glm::vec3 _scale, glm::vec3 _size, std::string _tag)
	{
		m_tag = _tag; // All meshes must have a tag for optimisation purposes in the collision
		m_shape = "mesh";
		m_mesh = new Mesh(_texName, _obj, _scale, _size);
		m_shapeComp = m_mesh;
		
	}

	void GameObject::addMeshShape(std::string _texName, glm::vec3 _scale, std::string _tag)
	{
		m_tag = _tag; // All meshes must have a tag for optimisation purposes in the collision
		m_shape = "mesh";
		m_mesh = new Mesh(_texName, _scale);
		m_shapeComp = m_mesh;
	
	}

	void GameObject::deleteShape()
	{
		m_tag = "untagged";
		m_shape = "NO SHAPE";
		delete m_plane;
		delete m_box;
		delete m_sphere;
		delete m_mesh;
	}

	void GameObject::addStats(float _hp, float _dmg, float _speed)
	{
		m_charSheet = new Stats(_hp, _dmg, _speed);
		m_stats = true;
	}

	void GameObject::addPhysics(std::string _tag, float _mass, float _bounciness)
	{
		m_rb = new PhysicsObj(_mass, _bounciness);
		m_physics = true;
		m_tag = _tag;
	}

	void GameObject::addAdvancedPhysics(std::string _shape, glm::vec3 _scale, float _mass)
	{
		m_Phy = new AdvancedPhysics(_shape, _scale, _mass);
		m_advancedPhys = true;
	}

	void GameObject::removePhysics()
	{
		m_physics = false;
		delete m_rb;
		m_advancedPhys = false;
		delete m_Phy;
	}
}
