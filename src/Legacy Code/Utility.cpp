
//#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
//#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "GameObject.h"
#include "Collision.h"
#include "Shader.h"
#include "Utility.h"
#include "Light.h"
#include "VertexArray.h"
#include "Camera.h"
#include "VertexBuffer.h"
#include "Input.h"
#include <time.h>

#define SHAPE(type) _obj->getShape() == type
#define OUTPUT(vec) std::cout << vec.x << " " << vec.y << " " << vec.z <<std::endl;
#define OTAG(tag) obj[i]->getTag() == tag
#define STAG(tag) sLight[i]->getTag() == tag

namespace Engine
{
	namespace utility
	{
		void onEveryFrame(Scene* _scene, float _dT) // This function runs every frame. Add various 'per frame' interactions here.
		{			
			GameObject* hand2 = nullptr;
			GameObject* cat = new GameObject();
			Input* input = _scene->m_core->m_input;
			Camera* camera = _scene->getCam();
			//GameObject* blood = nullptr;

			std::vector<GameObject*> obj = _scene->getObjects();
			std::vector<PointLight*> pLight = _scene->getpLight();
			std::vector<SpotLight*> sLight = _scene->getsLight();
			int numZter = 0;
			int numHornet = 0;

			for (int i = 0; i < obj.size(); i++) // Cycles through every object. You can access individual objects from its tag.
			{
				if (obj[i]->isActive())
				{

					if (OTAG("bulb"))
					{
						obj[i]->setPosition(camera->getPos() + camera->getFwd() * 2.5f + camera->getRight() * 1.1f - glm::vec3(0.0f, 1.0f, 0.0f));
						obj[i]->m_shapeComp->setRotation(glm::vec3(0.0f, camera->getYaw(), 0.0f));
						pLight[0]->setPos(obj[i]->getPosition());
					}

					else if (OTAG("hand2"))
					{
						hand2 = obj[i];
						hand2->setPosition((camera->getPos() - glm::vec3(0.0f, 1.7f, 0.0f)) + (camera->getFwd() * 3.0f) - (camera->getRight() * 1.1f));
						float turn;
						if (camera->getYaw() > 0.0f)
						{
							turn = 540.0f - camera->getYaw();
						}
						else
						{
							turn = 180.0f + -camera->getYaw();
						}
						hand2->m_shapeComp->setRotation(glm::vec3(0.0f, turn + 180.0f, 0.0f));
					}
					else if (OTAG("drop"))
					{
						float facing = atan2(camera->getPos().x - obj[i]->getPosition().x, camera->getPos().z - obj[i]->getPosition().z);
						obj[i]->m_shapeComp->setRotation(0.0f, glm::degrees(facing), 0.0f);
						if (obj[i]->getPosition().y < -15.0f)
						{
							obj[i]->setActive(false);
							obj[i]->setDelete(true);
						}
					}

					else if (OTAG("cat"))
					{
						cat = obj[i];
						if (cat->m_charSheet->getHP() <= 0.0f)
						{
							obj[i]->setActive(false);
							obj[i]->setDelete(true);							
						}

						glm::vec3 direction = glm::vec3(0, 0, 0);

						if (input->GetKeyIsDown(SDLK_a))
						{
							direction.x -= 1;
						}
						if (input->GetKeyIsDown(SDLK_d))
						{
							direction.x += 1;
						}
						if (input->GetKeyIsDown(SDLK_w))
						{
							direction.z += 1;
						}
						if (input->GetKeyIsDown(SDLK_s))
						{
							direction.z -= 1;
						}
						cat->m_charSheet->setDir(direction);

						glm::mat4 temp(1.0f);
						temp = glm::rotate(temp, glm::radians(-camera->getYaw()), glm::vec3(0, 1, 0));
						glm::vec3 directionFix;
						directionFix.z = direction.x;
						directionFix.x = direction.z;
						directionFix.y = direction.y;
						temp = glm::translate(temp, directionFix);
						glm::vec3 dir = temp * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

						if (dir != glm::vec3(0, 0, 0))
						{
							dir = glm::normalize(dir);
						}

						glm::vec3 pos = cat->getPosition();
						if (pos.y < 3.5f || pos.y > 3.5f)
						{
							pos.y = 3.5f;
							cat->setPosition(pos);
						}

						dir.y = 0.0f;
						if (cat->isActive())
						{
							if (cat->m_charSheet->getHP() < 100.0f)
							{
								cat->m_charSheet->addHP(0.05f);
							}

							utility::moveEntity(cat, dir, obj, _dT);
							camera->setPos(cat->getPosition() + glm::vec3(0.0f, 2.0f, 0.0f));
						}
					}


					if (obj[i]->hasStats())
					{
						float facing = atan2(camera->getPos().x - obj[i]->getPosition().x, camera->getPos().z - obj[i]->getPosition().z);
						glm::vec3 dir = -glm::normalize(obj[i]->getPosition() - cat->getPosition());


						if (obj[i]->m_charSheet->isHornet())
						{
							numHornet++;
							obj[i]->m_shapeComp->setRotation(0.0f, glm::degrees(facing), 0.0f);
							glm::vec3 posH = obj[i]->getPosition();
							if (posH.y < 6.0f)
							{
								posH.y = 6.0f;
							}
							//moveEntity(obj[i], dir, obj, _dT);
						}

						else if (obj[i]->m_charSheet->isZter())
						{
							numZter++;
							obj[i]->m_shapeComp->setRotation(0.0f, glm::degrees(facing), 0.0f);
							//moveEntity(obj[i], dir, obj, _dT);
						}

						if (obj[i]->m_charSheet->getHP() <= 0.0f)
						{
							numHornet++;
							obj[i]->setActive(false);
							obj[i]->setDelete(true);
						}
					}

				}
			}
			
			for (int i = 0; i < sLight.size(); i++)
			{
				if (STAG("torch"))
				{
					sLight[i]->setPos(camera->getPos() + camera->getFwd() * 0.5f);
					sLight[i]->setDirection(camera->getFwd());
				}
				else if (STAG("blotch"))
				{
					sLight[i]->setPos(camera->getPos() + camera->getFwd() * 0.5f);
					sLight[i]->setDirection(camera->getFwd() + camera->getRight() * glm::vec3(0.1, 0.1, 0.1));
				}
			}
		}

		void moveEntity(GameObject* _obj, glm::vec3 _dir, std::vector<GameObject*> _other, float _dTs)
		{
			for (int i = 0; i < _other.size(); i++)
			{
				if (_other[i]->m_shapeComp->isSolid() && _other[i]->isActive() && _other[i]->getTag() != _obj->getTag())
				{
					if (Physics::meshToMesh(_obj, _other[i], glm::vec3(0.0f)))
					{
						glm::vec3 direction = glm::normalize(_obj->getPosition() - _other[i]->getPosition());
						Collision* col = new Collision(_obj->getPosition(), direction, _other[i], _obj);
						Physics::onCollision(col);
						delete col;
						_dir = direction;
						break;
					}
				}
			}
			_obj->setPosition(_obj->getPosition() + _dir * _obj->m_charSheet->getSpeed() * _dTs);
		}

		void update(GameObject* _obj, std::shared_ptr<Shader> _shader, std::vector<GameObject*> _allObj, float _dT)
		{
			glm::mat4 modelMat(1.0f); // Model Matrix
			modelMat = glm::translate(modelMat, _obj->getPosition()); // Translate by game object's position

			if (_obj->isAdvancedPhysics())
			{
				if (_obj->getTag() != "testbox")
				{
					glm::mat4 rotationMat = glm::mat4(_obj->m_Phy->getRotMat());
					glm::quat rotate = glm::normalize(glm::quat_cast(rotationMat));
					_obj->m_Phy->setRotMat(glm::mat3_cast(rotate));

					modelMat *= glm::mat4_cast(rotate);
				}
				else
				{
					glm::mat4 rotMatNew = glm::mat4_cast(_obj->m_Phy->m_rotQuat);	
					modelMat *= rotMatNew;
				}
			}

			else
			{
				glm::vec3 rotation = glm::radians(_obj->m_shapeComp->getRot());
				{
					modelMat = glm::rotate(modelMat, rotation.x, glm::vec3(1, 0, 0)); //Rotate by the shape component's rotation
					modelMat = glm::rotate(modelMat, rotation.y, glm::vec3(0, 1, 0));
					modelMat = glm::rotate(modelMat, rotation.z, glm::vec3(0, 0, 1));
				}
			}
			modelMat = glm::scale(modelMat, _obj->m_shapeComp->getScale()); // Scale by shape object's scale

			_obj->m_shapeComp->setModel(modelMat); // Send the model matrix back to the shape component to be drawn	

			_shader->setUniform("in_Model", _obj->m_shapeComp->getModel()); // Translate the model matrix by camera position and stuff
			_shader->setUniform("in_TransverseModel", glm::transpose(glm::inverse(_obj->m_shapeComp->getModel())));
			_shader->setUniform("in_Texture", _obj->m_shapeComp->m_tex);

			if (_obj->isPhysics())
			{
				if (_obj->getTag() == "testbox")
				{
					_obj->m_Phy->m_rotQuat = glm::rotate(_obj->m_Phy->m_rotQuat, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
				}
				else
				{
					Physics::update(_obj, _allObj, _dT); // Also update the shape's rigidbody if it has one. Checks for collisions and adjusts position
				}
			}			
		}

		void updateLighting(std::vector<SpotLight*> _sLight, std::vector<DirLight*> _dLight, std::vector<PointLight*> _pLight, std::shared_ptr<Shader> _lSh, std::shared_ptr<Shader> _iSh)
		{
			SpotLight* torch = nullptr;
			for (int i = 0; i < _sLight.size(); i++)
			{
				if (_sLight[i]->getTag() == "torch")
				{
					torch = _sLight[i];
				}
			}
			std::string uniform;

			//Generic Lighting shader

			//For each light type
			for (int i = 0; i < _pLight.size(); i++)
			{
				std::string itr = std::to_string(i);
				uniform = "in_pLight[" + itr + "].m_specIntens";
				_lSh->setUniform(uniform, _pLight[i]->getSpec());

				uniform = "in_pLight[" + itr + "].m_diffuse";
				_lSh->setUniform(uniform, _pLight[i]->getDif());


				uniform = "in_pLight[" + itr + "].m_pos";
				_lSh->setUniform(uniform, _pLight[i]->getPos());

				uniform = "in_pLight[" + itr + "].m_radius";
				_lSh->setUniform(uniform, _pLight[i]->getRadius());

				uniform = "in_pLight[" + itr + "].m_quadratic";
				_lSh->setUniform(uniform, _pLight[i]->getQuad());

			}

			for (int i = 0; i < _dLight.size(); i++)
			{
				std::string itr = std::to_string(i);
				uniform = "in_dLight[" + itr + "].m_specIntens";
				float t = _dLight[i]->getSpec();
				_lSh->setUniform(uniform, _dLight[i]->getSpec());

				uniform = "in_dLight[" + itr + "].m_diffuse";
				_lSh->setUniform(uniform, _dLight[i]->getDif());

				uniform = "in_dLight[" + itr + "].m_ambient";
				_lSh->setUniform(uniform, _dLight[i]->getAmb());

				uniform = "in_dLight[" + itr + "].m_direction";
				_lSh->setUniform(uniform, _dLight[i]->getDirection());
			}

			for (int i = 0; i < _sLight.size(); i++)
			{
				std::string itr = std::to_string(i);
				uniform = "in_sLight[" + itr + "].m_specIntens";
				_lSh->setUniform(uniform, _sLight[i]->getSpec());

				uniform = "in_sLight[" + itr + "].m_diffuse";
				_lSh->setUniform(uniform, _sLight[i]->getDif());

				uniform = "in_sLight[" + itr + "].m_pos";
				_lSh->setUniform(uniform, _sLight[i]->getPos());

				uniform = "in_sLight[" + itr + "].m_radius";
				_lSh->setUniform(uniform, _sLight[i]->getRadius());

				uniform = "in_sLight[" + itr + "].m_quadratic";
				_lSh->setUniform(uniform, _sLight[i]->getQuad());

				uniform = "in_sLight[" + itr + "].m_angle";
				_lSh->setUniform(uniform, glm::cos(glm::radians(_sLight[i]->getAngle())));

				uniform = "in_sLight[" + itr + "].m_fadeAngle";
				_lSh->setUniform(uniform, glm::cos(glm::radians(_sLight[i]->getFangle())));

				uniform = "in_sLight[" + itr + "].m_direction";
				_lSh->setUniform(uniform, _sLight[i]->getDirection());

				uniform = "in_sLight[" + itr + "].m_antiLight";
				_lSh->setUniform(uniform, _sLight[i]->isAnti());
			}

			////Monster shader
			//uniform = "in_sLight.m_specIntens";
			//_mSh->setUniform(uniform, torch->getSpec());

			//uniform = "in_sLight.m_pos";
			//_mSh->setUniform(uniform, torch->getPos());

			//uniform = "in_sLight.m_radius";
			//_mSh->setUniform(uniform, torch->getRadius());

			//uniform = "in_sLight.m_quadratic";
			//_mSh->setUniform(uniform, torch->getQuad());

			//uniform = "in_sLight.m_angle";
			//_mSh->setUniform(uniform, glm::cos(glm::radians(torch->getAngle())));

			//uniform = "in_sLight.m_fadeAngle";
			//_mSh->setUniform(uniform, glm::cos(glm::radians(torch->getFangle())));

			//uniform = "in_sLight.m_direction";
			//_mSh->setUniform(uniform, torch->getDirection());

			//uniform = "in_sLight.m_time";

			//float t = SDL_GetTicks();
			//_mSh->setUniform(uniform, t);

			////Hornet shader
			//uniform = "in_sLight.m_specIntens";
			//_iSh->setUniform(uniform, torch->getSpec());

			//uniform = "in_sLight.m_pos";
			//_iSh->setUniform(uniform, torch->getPos());

			//uniform = "in_sLight.m_radius";
			//_iSh->setUniform(uniform, torch->getRadius());

			//uniform = "in_sLight.m_quadratic";
			//_iSh->setUniform(uniform, torch->getQuad());

			//uniform = "in_sLight.m_angle";
			//_iSh->setUniform(uniform, glm::cos(glm::radians(torch->getAngle())));

			//uniform = "in_sLight.m_fadeAngle";
			//_iSh->setUniform(uniform, glm::cos(glm::radians(torch->getFangle())));

			//uniform = "in_sLight.m_direction";
			//_iSh->setUniform(uniform, torch->getDirection());
			//uniform = "in_sLight.m_diffuse";
			//_iSh->setUniform(uniform, torch->getDif());
		}

		

		//Copy a game object function
		GameObject * copy(GameObject * _obj)
		{
			GameObject* t = new GameObject();

			if (_obj->getShape() != "NO SHAPE")
			{
				t->m_shapeComp = new Shape();
				*t->m_shapeComp = *_obj->m_shapeComp;

				if (SHAPE("sphere"))
				{
					t->m_sphere = new Sphere();
					*t->m_sphere = *_obj->m_sphere;
				}
				else if (SHAPE("plane"))
				{
					t->m_plane = new Plane();
					*t->m_plane = *_obj->m_plane;
				}
				else if (SHAPE("mesh"))
				{
					t->m_mesh = new Mesh();
					*t->m_mesh = *_obj->m_mesh;
				}
				else if (SHAPE("box"))
				{
					t->m_box = new Box();
					*t->m_box = *_obj->m_box;
				}
				if (_obj->isPhysics())
				{
					t->m_rb = new PhysicsObj();
					*t->m_rb = *_obj->m_rb;
				}
				if (_obj->isAdvancedPhysics())
				{
					t->m_Phy = new AdvancedPhysics();
					*t->m_Phy = *_obj->m_Phy;
				}
				if (_obj->hasStats())
				{
					t->m_charSheet = new Stats(0.0f, 0.0f, 0.0f);
					*t->m_charSheet = *_obj->m_charSheet;
					t->setStats(true);
				}

			}
			t->setShader(_obj->getShader());
			t->setPosition(_obj->getPosition());
			t->setTag(_obj->getTag() + "[copy]");
			t->setActive(_obj->isActive());
			t->setShapeString(_obj->getShape());
			t->setPhysBool(_obj->isPhysics());
			t->setAdPhysBool(_obj->isAdvancedPhysics());
			t->setStats(_obj->hasStats());
			return t;
		}

		float lerp(float _a, float _b, float _f)
		{
			return _a + _f * (_b - _a);
		}		
	}
}

