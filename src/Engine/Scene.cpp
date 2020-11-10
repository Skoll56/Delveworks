#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include <string>
#include "Scene.h"
#include "Input.h"
#include "Shader.h"
#include "Camera.h"
#include "Light.h"
#include "Utility.h"
#include "Physics.h"
#include "Resource.h"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
namespace Engine
{
	Scene::Scene()
	{
	}

	//Creates a room around a given point using 6 walls
	void Scene::createRoom(float _x, float _y, float _z, glm::vec3 _pos)
	{
		Room* room = new Room;
		room->m_pos = _pos - glm::vec3(0.0f, _pos.y / 2.0f, 0.0f);
		std::vector<GameObject*> wall;
		wall.resize(6);
		for (int i = 0; i < 6; i++)
		{
			wall[i] = utility::copy(m_templateWall);
			wall[i]->setTag(std::to_string(i) + "w");
			wall[i]->setRoom(m_roomNum);
		}
		wall[0]->setTag("roof");
		wall[1]->setTag("floor");
		glm::vec3 pos;

		// Roof and floor	
		pos.x = _pos.x;
		pos.y = _pos.y + (_y / 2.0f);
		pos.z = _pos.z;
		wall[0]->setPosition(pos);
		wall[0]->m_shapeComp->setScale(_x + 2.0f, 0.001f, _z + 2.0f);



		pos.y = _pos.y - (_y / 2.0f);
		wall[1]->setPosition(pos);
		wall[1]->m_shapeComp->setScale(_x + 2.0f, 0.001f, _z + 2.0f);


		//X pair
		pos.x = _pos.x + (_x / 2.0f);
		pos.y = _pos.y;
		pos.z = _pos.z;
		wall[2]->setPosition(pos);
		wall[2]->m_shapeComp->setScale(1.0f, _y + 2.0f, _z + 2.0f);



		pos.x = _pos.x - (_x / 2.0f);
		wall[3]->setPosition(pos);
		wall[3]->m_shapeComp->setScale(1.0f, _y + 2.0f, _z + 2.0f);


		//Z pair	
		pos.x = _pos.x;
		pos.y = _pos.y;
		pos.z = (_pos.z) + _z / 2.0f;
		wall[4]->setPosition(pos);
		wall[4]->m_shapeComp->setScale(_x + 2.0f, _y + 2.0f, 1.0f);


		pos.z = _pos.z - (_z / 2.0f);
		wall[5]->setPosition(pos);
		wall[5]->m_shapeComp->setScale(_x + 2.0f, _y + 2.0f, 1.0f);

		room->m_walls = wall;
		for (int i = 0; i < wall.size(); i++)
		{
			m_object.push_back(wall[i]);
		}

		room->m_id = m_roomNum;
		m_roomNum++;
		m_room.push_back(room);
	}

	void Scene::createRoom(float _x, float _y, float _z, int _NESW, Room * _prev)
	{
		//return new Room();
	}

	//Initialise the starting scene with various objects
	void Scene::createStartScene()
	{
		m_lightingSh = m_core->m_rManager->load<Shader>("light");
		m_uiSh = m_core->m_rManager->load<Shader>("UI");

		//Spheres are size 1/1/1
		//Boxes are size 1/1/1
		//Planes are size 1/0/1 
		//Meshes may vary. Mesh sizes are used to determine their hitbox (box collisions are always tested before the mesh collision for efficiency)
		m_templateWall = new GameObject(glm::vec3(5000.0f, 0.0f, 0.0f));
		m_templateWall->addBoxShape("diffuse.bmp", glm::vec3(1.0f));
		m_templateWall->setTag("wall");
		m_templateWall->setShader(m_lightingSh);
		m_templateWall->m_shapeComp->setShine(4);

		createRoom(100.0f, 50.0f, 100.0f, glm::vec3(0, 25.0f, 100.0f));		
		
		
		std::cout << "Successfully loaded shaders" << std::endl;

		m_mainCam = new Camera(glm::vec3(-0, 5, -30));

		GameObject *player = new GameObject(glm::vec3(20.0f, 3.5f, 0.0f));
		player->addBoxShape("default.bmp", glm::vec3(2.0f, 6.0f, 2.0f));
		player->setTag("cat");
		player->addStats(100.0f, 20.0f, 10.0f);
		player->setShader(m_lightingSh);
		addToRoom(player, 0);

		GameObject * statue = new GameObject(glm::vec3(0.0f, -10.0f, 0.0f));
		statue->addMeshShape("statue_diffuse.png", "statue.obj", glm::vec3(2.0f, 1.5f, 2.0f), glm::vec3(5.0f, 10.0f, 5.0f), "statue");
		statue->m_shapeComp->setShine(1);
		statue->setShader(m_lightingSh);
		addToRoom(statue, 0);

		GameObject* cube = new GameObject(glm::vec3(0.0f, 1.0f, 0.0f));
		cube->addBoxShape("diffuse.bmp", glm::vec3(2.0f, 2.0f, 2.0f));
		cube->setShader(m_lightingSh);
		addToRoom(cube, 0);

		m_object.push_back(statue);
		m_object.push_back(cube);
		m_object.push_back(player);

		//Lights
		PointLight *bulb = new PointLight(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.3f, 0.3f), 0.6f, 32.0f, 0.07f);
		DirLight *sun = new DirLight(glm::vec3(0.5f, 0.5f, 0.5f), 0.4f, glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.3f, -1.0f, 0.0f));
		SpotLight *torch = new SpotLight(glm::vec3(-25.0f, 20.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.8f, 10.0f, 15.0f, 160.0f, 0.027f, glm::vec3(0.0f, -1.0f, 0.0f));
		torch->setTag("torch");
		SpotLight *blotch = new SpotLight(glm::vec3(25.0f, 20.0f, 0.0f), glm::vec3(-0.4f, -0.4f, -0.4f), 0.8f, 3.0f, 7.0f, 100.0f, 0.0075f, glm::vec3(0.0f, -1.0f, 0.0f));
		blotch->setAnti(1);
		blotch->setTag("blotch");

		m_pLight.push_back(bulb);
		m_dLight.push_back(sun);
		m_sLight.push_back(torch);
		m_sLight.push_back(blotch);
	}

	void Scene::update(float _dT)
	{
		///Camera		
		m_mainCam->update(_dT, m_core->m_input); // Update camera based on the input
		//m_simpleSh->setUniform("in_Projection", glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f)); //Set the projection uniform
		m_lightingSh->setUniform("in_Projection", glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f)); //Set the projection uniform
		//m_monsterSh->setUniform("in_Projection", glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f)); //Set the projection uniform
		m_uiSh->setUniform("in_Projection", glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f)); //Set the projection uniform
		//m_invSh->setUniform("in_Projection", glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f)); //Set the projection uniform
		glm::mat4 model(1.0f);
		model = glm::lookAt(m_mainCam->getPos(), m_mainCam->getPos() + m_mainCam->getFwd(), m_mainCam->getUp());

		//m_simpleSh->setUniform("in_View", model); // Establish the view matrix		
		m_uiSh->setUniform("in_View", model); // Establish the view matrix	
		m_lightingSh->setUniform("in_View", model); // Establish the view matrix
		//m_monsterSh->setUniform("in_View", model); // Establish the view matrix
		//m_invSh->setUniform("in_View", model); // Establish the view matrix

		//Make various per-frame changes
		utility::onEveryFrame(this, _dT);

		//Lighting Shaders
		m_lightingSh->setUniform("in_Emissive", glm::vec3(0.0f, 0.0f, 0.0f));
		//m_invSh->setUniform("in_Emissive", glm::vec3(0.8f, 0.0f, 0.0f));
		m_lightingSh->setUniform("in_CamPos", m_mainCam->getPos());
		//m_monsterSh->setUniform("in_CamPos", m_mainCam->getPos());
		//m_invSh->setUniform("in_CamPos", m_mainCam->getPos());

		utility::updateLighting(m_sLight, m_dLight, m_pLight, m_lightingSh, m_invSh);

		for (int o = 0; o < m_object.size(); o++)
		{
			if (m_object[o]->getDelete()) //Delete the objects at the start of the frame or loops will get damaged and memory violations happen
			{
				delete m_object[o];
				m_object[o] = nullptr;
				m_object.erase(m_object.begin() + o);
			}
		}

		///Objects				
		for (int o = 0; o < m_object.size(); o++) // Check every object
		{
			if (m_object[o]->isActive() && m_object[o]->getShape() != "NO SHAPE") // Don't bother updating objects with no collider or mesh
			{
				if (m_object[o]->isPhysics())
				{
					if (m_object[o]->m_rb->collided()) // Checks for any objects that collided during their update earlier
					{
						// Always handle collisions BEFORE updating physics positions because this function adjusts velocity due to response
						Physics::handleCollisions(m_object[o]);
					}
				}

				m_lightingSh->setUniform("in_Shininess", (int)pow(2, m_object[o]->m_shapeComp->getShine()));
				utility::update(m_object[o], m_object[o]->getShader(), m_object, _dT); // Update the object's model matrix and appearance, and its physical location
				if (m_object[o]->getTag() != "blood")
				{
					m_object[o]->getShader()->draw(m_object[o]->m_shapeComp->m_vAO); // Draw the object to the screen using its own shader
				}
			}
		}
		for (int o = 0; o < m_object.size(); o++)
		{
			if (m_object[o]->getTag() == "blood") // This needs to be drawn last, along with any other transparent objects
			{
				m_object[o]->getShader()->draw(m_object[o]->m_shapeComp->m_vAO);
			}
		}

	}

	void Scene::addToRoom(GameObject * _obj, int _room)
	{
		_obj->setPosition(_obj->getPosition() + m_room.at(_room)->m_pos);
		_obj->setRoom(m_room.at(_room)->m_id);
	}




	//https://sketchfab.com/models/ed37306f40c547c1a179587665b54286?cursor=cD0yMDE3LTA3LTI3KzIyJTNBMDAlM0E1Mi44MDkxNDM%3D // Wyrm model
		//https://sketchfab.com/models/ab12bf5debaf482db6456326e2799969?ref=related // Snake Man
			// https://sketchfab.com/models/f2b2894395864d49b12ca0065f30ade2?ref=related#download // Cthulhu Statue
			// https://www.turbosquid.com/FullPreview/Index.cfm/ID/1082345//  Gun

}