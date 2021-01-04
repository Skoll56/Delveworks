#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include "Collision.h"
#include "PhysicsNew.h"
#include <math.h>
#include <stdio.h>
#include <typeinfo.h>
#include "MeshRenderer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Collider.h"

#define WITHIN(n1, n2, var) n1 > n2 - var && n1 < n2 + var
#define UP glm::vec3(0.0f, 1.0f, 0.0f)
#define DOWN glm::vec3(0.0f, -1.0f, 0.0f)
#define LEFT glm::vec3(1.0f, 0.0f, 0.0f)
#define RIGHT glm::vec3(-1.0f, 0.0f, 0.0f)
#define FORWARDS glm::vec3(0.0f, 0.0f, 1.0f)
#define BACKWARDS glm::vec3(0.0f, 0.0f, 1.0f) //Yes, beckwards is the same as forwards. I don't get it either but it works.

namespace Engine
{

	void PhysicsEventUser::onTick()
	{
		std::shared_ptr<Transform> trans = getEntity()->getComponent<Transform>();			
		float _dT = getEntity()->getCore()->getDeltaTime();
		setCollided(false); //We do this here instead of in the 'Reset collision function' because the rotational update needs to know
		glm::vec3 nextPos = trans->getPosition();
		bool collided = checkForCollisions(_dT, getEntity()->getCore()->getAllEntities(), nextPos); // Check Collisions
	}

	//Tests for collisions between pairs of shapes
	bool PhysicsEventUser::checkForCollisions(float _dT, std::vector<std::shared_ptr<Entity>> _objects, glm::vec3 _c1)
	{
		setFloored(false); //The floor is almost always the first object this checks, so if we're still colliding
								  //With the floor, this should be 'true' for the following checks.		
		for (int i = 0; i < _objects.size(); i++) // Check every object
		{
			std::shared_ptr<SphereCollider> s = _objects[i]->getComponent<SphereCollider>();
			std::shared_ptr<MeshCollider> m = _objects[i]->getComponent<MeshCollider>();
			std::shared_ptr<BoxCollider> b = _objects[i]->getComponent<BoxCollider>();
			std::shared_ptr<PlaneCollider> p = _objects[i]->getComponent<PlaneCollider>();
			bool hasCollider = false;
			if (s || m || b || p) 
			{ 
				hasCollider = true; 
			}

			if (hasCollider && _objects[i]->isActive()) // Don't check collisions against inactive objects and against non-solids
			{
				if (_objects[i]->getTag() != getEntity()->getTag()) // Don't collide with yourself
				{
					std::shared_ptr<Entity> self = getEntity();

					std::string otherShape;
					if (s) { otherShape = "sphere"; }
					else if (m) { otherShape = "mesh"; }
					else if (b) { otherShape = "box"; }
					else if (p) { otherShape = "plane"; }

					std::shared_ptr<SphereCollider> s2 = self->getComponent<SphereCollider>();
					std::shared_ptr<MeshCollider> m2 = self->getComponent<MeshCollider>();
					std::shared_ptr<BoxCollider> b2 = self->getComponent<BoxCollider>();
					std::shared_ptr<PlaneCollider> p2 = self->getComponent<PlaneCollider>();
					std::string myShape; // My object type
					if (s2) { myShape = "sphere"; }
					else if (m2) { myShape = "mesh"; }
					else if (b2) { myShape = "box"; }
					else if (p2) { myShape = "plane"; }

					bool collision = false;
					if (myShape == "sphere") // If current object is a sphere
					{
						//Y + P
						if (otherShape == "sphere")
						{
							collision = sphereToSphere(s2, s);
							if (collision) { setCollided(true); }
						}

						//Y + P
						else if (otherShape == "plane")
						{
							collision = sphereToPlane(s2, p, _c1);
							if (collision) 
							{ 
								setCollided(true); 
							}

						}

						else if (otherShape == "mesh")
						{
							//Spheres have a high polygon count. Tri-sphere is needed here.
						}

						else if (otherShape == "box")
						{
							collision = sphereToBox(s2, b, _c1);
							if (collision) { setCollided(true); }

						}
					}

					else if (myShape == "box")
					{
						if (otherShape == "sphere")
						{
							collision = boxToSphere(b2, s, _c1);
							if (collision)
							{
								setCollided(true);
							}
						}

						//Y + P
						else if (otherShape == "plane")
						{
							collision = boxToPlane(b2, p, _c1);
							if (collision)
							{
								setCollided(true);
							}
						}

						else if (otherShape == "mesh")
						{
							//Tri box is needed here
						}

						else if (otherShape == "box")
						{
							collision = boxToBox(b2, b, _c1);
							if (collision)
							{
								setCollided(true);
							}
						}
					}

					else if (myShape == "mesh")
					{
						collision = meshToMesh(m2, m, _c1);
						if (collision) { setCollided(true); }

					}  // End of collision code	


				}
			}
		}
		
		return false;
	}

	float PhysicsEventUser::distanceToPlane(glm::vec3 & _n, glm::vec3 & _p, glm::vec3 & _q)
	{
		return glm::dot((_p - _q), _n); //This function has been provided by Professor Tang.
	}

	///Shape Intersection functions

	//Works with physics
	bool PhysicsEventUser::sphereToPlane(std::shared_ptr<SphereCollider> _my, std::shared_ptr<PlaneCollider> _other, glm::vec3 _c1)
	{
		glm::vec3 n = _other->getNorm();
		glm::vec3 q = _other->transform()->getPosition();
		glm::vec3 c0 = _my->transform()->getPosition();
		glm::vec3 permCP = getPermCP();
		glm::vec3 ci;

		float r = _my->getRadius();
		float dis2Plane = distanceToPlane(n, c0, q);
		float dis2Plane2 = distanceToPlane(n, _c1, q);
		glm::vec3 length = (_other->transform()->getScale() / 2.0f);
		float a, b, c;
		a = b = c = 0.0f;
		a = 1.0f - fabs(n.x);
		b = 1.0f - fabs(n.y);
		c = 1.0f - fabs(n.z);
		glm::vec3 invNormal = glm::vec3(a, b, c); //Checks the values that aren't the normal (Ie: Are in the plane's direction)

												  //This creates 'adjacent' values. We want to know if we move adjacenetly off the plane in whatever direction it faces
		glm::vec3 aLength = length * invNormal;
		glm::vec3 aPos = c0 * invNormal;
		glm::vec3 aPpos = q * invNormal;
		glm::vec3 aRadius = r * invNormal;


		if (fabs(dis2Plane) <= r) //If already colliding with the plane
		{
			if (aPos.x > aPpos.x + aLength.x || aPos.y > aPpos.y + aLength.y || aPos.z > aPpos.z + aLength.z) // If we fall off the plane
			{
				return false;
			}
			glm::vec3 d2p = dis2Plane * n;
			d2p -= r * n;
			ci = c0 - d2p;
			m_collisions.push_back(std::make_shared<Collision>(ci, n, _other->getEntity(), _my->getEntity()));
			
			return true;
		}


		else { return false; }
	}

	//Works with physics
	bool PhysicsEventUser::sphereToSphere(std::shared_ptr<SphereCollider> _my, std::shared_ptr<SphereCollider> _other)
	{
		glm::vec3 c0 = _my->transform()->getPosition();
		glm::vec3 c1 = _other->transform()->getPosition();
		float r1 = _my->getRadius();
		float r2 = _other->getRadius();
		float d = glm::length(c0 - c1);

		if (d <= r1 + r2)
		{
			glm::vec3 sN = glm::normalize(c0 - c1);
			glm::vec3 cp = (r1 * sN) + (c1 + r2 * sN);
			m_collisions.push_back(std::make_shared<Collision>(cp, sN, _other->getEntity(), _my->getEntity()));
			return true;
		}
		else { return false; }
	}

	//Works
	bool PhysicsEventUser::boxToSphere(std::shared_ptr<BoxCollider> _my, std::shared_ptr<SphereCollider> _other, glm::vec3 _c1)
	{		
		if (sphereBoxOverlap(_other, _my))
		{
			std::shared_ptr<AdvPhysicsObject> RB = _my->getEntity()->getComponent<AdvPhysicsObject>();
			glm::vec3 otherP = _other->transform()->getPosition();
			glm::vec3 pos = _my->transform()->getPosition();
			glm::vec3 length, oLength;
			std::vector<glm::vec3> normals;
			normals.resize(6);
			normals[0] = UP;
			normals[1] = DOWN;
			normals[2] = LEFT;
			normals[3] = RIGHT; //Need to multiply by rotmat
			normals[4] = FORWARDS;
			normals[5] = BACKWARDS;


			//Other object's length
			oLength = _other->getRadius() * _other->transform()->getSize(); //Should be 1, but just incase.
			if (!RB)
			{
				length = ((_my->transform()->getScale() / 2.0f) * _my->transform()->getSize());
			}
			else
			{
				length = ((_my->transform()->getScale() / 2.0f) * _my->transform()->getSize()) * RB->getRotMat(); //Experimental
			}
			float lowest = INFINITY;
			int winFace;
			for (int i = 0; i < 6; i++) // For each face
			{
				glm::vec3 pL = otherP + (oLength * normals[i]); //Check which direction is the best suited for this collision
				glm::vec3 sL = pos - (length * normals[i]);
				float dToFace = fabs(distanceToPlane(normals[i], sL, pL));

				if (dToFace < lowest) // Check which face most closely 'faces' us
				{
					lowest = dToFace;
					winFace = i; //We can only have collided with the face that faces us. 
				}
			}
			glm::vec3 n = normals[winFace];
			if (pos.z < otherP.z)
			{
				n.z = -n.z;
			}

			glm::vec3 expected = (otherP + (oLength * n) + (length * n)); //Expected position in the normal
			glm::vec3 dif = ((pos * n) - (expected * n)) * n; //Difference between ^ and actual position
			glm::vec3 cp = pos - dif; //Aforementioned difference saved and ready to be applied
			m_collisions.push_back(std::make_shared<Collision>(cp, n, _other->getEntity(), _my->getEntity()));
			return true;

		}
		return false;
	}

	bool PhysicsEventUser::sphereToBox(std::shared_ptr<SphereCollider> _my, std::shared_ptr<BoxCollider> _other, glm::vec3 _c1)
	{

		if (sphereBoxOverlap(_my, _other))
		{			
			glm::vec3 otherP = _other->transform()->getPosition();
			glm::vec3 pos = _my->transform()->getPosition();
			glm::vec3 length, oLength;

			length = _my->getRadius() * _my->transform()->getSize(); //Should be 1, but just incase.

			std::shared_ptr<AdvPhysicsObject> oRB = _other->getEntity()->getComponent<AdvPhysicsObject>();
			if (!oRB)
			{
				oLength = ((_other->transform()->getScale() / 2.0f) * _other->transform()->getSize());
			}
			else
			{
				oLength = ((_other->transform()->getScale() / 2.0f) * _other->transform()->getSize()) * oRB->getRotMat(); //Experimental
			}
			std::vector<glm::vec3> oNormals;
			oNormals.resize(6);
			oNormals[0] = UP;
			oNormals[1] = DOWN;
			oNormals[2] = LEFT;
			oNormals[3] = RIGHT; //Multiply by rotmat
			oNormals[4] = FORWARDS;
			oNormals[5] = BACKWARDS;
			glm::vec3 rayToBox = pos - otherP;

			float lowest = INFINITY;
			int winFace;

			for (int i = 0; i < 6; i++) // For each face
			{
				glm::vec3 pL = otherP + (oLength * oNormals[i]);
				glm::vec3 sL = pos - (length * oNormals[i]);
				float dToFace = fabs(distanceToPlane(oNormals[i], sL, pL));

				if (dToFace < lowest) // Check which face most closely 'faces' us
				{
					lowest = dToFace;
					winFace = i; //We can only have collided with the face that faces us. 
				}
			}
			if (winFace == 4 || winFace == 5)
			{
				int r = 5;
			}

			glm::vec3 n = oNormals[winFace];

			if (pos.z < otherP.z)
			{
				n.z = -n.z;
			}
			//The 'Z' axis is visually backwards

			glm::vec3 expected = (otherP + (oLength * n) + (length * n));//Expected position in the normal
			glm::vec3 dif = ((pos * n) - (expected * n)) * n;//Difference between ^ and actual position
			glm::vec3 cp = pos - dif;//Aforementioned difference saved and ready to be applied

			m_collisions.push_back(std::make_shared<Collision>(cp, n, _other->getEntity(), _my->getEntity())); //Create a collision
			return true;
		}
		return false;
	}

	glm::vec3 PhysicsEventUser::roundNormals(glm::vec3 _n)
	{
		if (_n.x > 0.95f) { _n.x = 1.0f; }
		else if (_n.x < -0.95f) { _n.x = -1.0f; }
		else if (WITHIN(_n.x, 0, 0.05f)) { _n.x = 0.0f; }

		if (_n.y > 0.95f) { _n.y = 1.0f; }
		else if (_n.y < -0.95f) { _n.y = 1.0f; } // This one stays positive. We don't need ceilings.
		else if (WITHIN(_n.y, 0, 0.05f)) { _n.y = 0.0f; }

		if (_n.z > 0.95f) { _n.z = 1.0f; }
		else if (_n.z < -0.95f) { _n.z = -1.0f; }
		else if (WITHIN(_n.z, 0, 0.05f)) { _n.z = 0.0f; }
		return _n;
	}

	//Works with physics
	bool PhysicsEventUser::boxToBox(std::shared_ptr<BoxCollider> _my, std::shared_ptr<BoxCollider> _other, glm::vec3 _c1)
	{
		glm::vec3 otherP = _other->transform()->getPosition();
		glm::vec3 pos = _my->transform()->getPosition();
		glm::vec3 length, oLength;
		std::vector<glm::vec3> oNormals;
		oNormals.resize(6);
		oNormals[0] = UP;
		oNormals[1] = DOWN;
		oNormals[2] = LEFT;
		oNormals[3] = RIGHT; //Multiply by rotmat
		oNormals[4] = FORWARDS;
		oNormals[5] = BACKWARDS;

		std::shared_ptr<AdvPhysicsObject> oRB = _other->getEntity()->getComponent<AdvPhysicsObject>();
		std::shared_ptr<AdvPhysicsObject> RB = _my->getEntity()->getComponent<AdvPhysicsObject>();
		if (!RB)
		{
			length = ((_my->transform()->getScale() / 2.0f) * _my->transform()->getSize()); // We factor size into the calculations on the off-chance it isn't 1. It should be 1 by default
		}
		else
		{
			length = ((_my->transform()->getScale() / 2.0f) * _my->transform()->getSize()) * RB->getRotMat(); //Experimental
		}

		if (!oRB)
		{
			oLength = ((_other->transform()->getScale() / 2.0f) * _other->transform()->getSize());
		}

		else
		{
			oLength = ((_other->transform()->getScale() / 2.0f) * _other->transform()->getSize()) * oRB->getRotMat(); //Experimental
		}


		if (pos.x + length.x >= otherP.x - oLength.x && pos.x - length.x <= otherP.x + oLength.x)
		{
			if (pos.y + length.y >= otherP.y - oLength.y && pos.y - length.y <= otherP.y + oLength.y)
			{
				if (pos.z + length.z >= otherP.z - oLength.z && pos.z - length.z <= otherP.z + oLength.z)
				{
					glm::vec3 rayToBox = pos - otherP;

					float lowest = INFINITY;
					int winFace;
					for (int i = 0; i < 6; i++) // For each face
					{
						glm::vec3 pL = otherP + (oLength * oNormals[i]);
						glm::vec3 sL = pos - (length * oNormals[i]);
						float dToFace = fabs(distanceToPlane(oNormals[i], sL, pL));

						if (fabs(dToFace) < lowest) // Check which face most closely 'faces' us
						{
							lowest = dToFace;
							winFace = i; //We can only have collided with the face that faces us. 
						}
					}
					glm::vec3 n = oNormals[winFace];
					if (pos.z < otherP.z)
					{
						n.z = -n.z;
					}
					glm::vec3 expected = (otherP + (oLength * n) + (length * n));
					glm::vec3 dif = ((pos * n) - (expected * n)) * n;
					glm::vec3 cp = pos - dif;

					if (n == glm::vec3(0.0f, 1.0f, 0.0f)) // We're on top of the box
					{
						setFloored(true);
					}

					m_collisions.push_back(std::make_shared<Collision>(cp, n, _other->getEntity(), _my->getEntity()));
					return true;
					//Needs to find the normal, the CP and create the collision
				}
			}
		}
		return false;
	}

	//Works
	bool PhysicsEventUser::boxToPlane(std::shared_ptr<BoxCollider> _my, std::shared_ptr<PlaneCollider> _other, glm::vec3 _c1)
	{
		std::shared_ptr<PhysicsEventUser> rb = _my->getEntity()->getComponent<PhysicsEventUser>();
		glm::vec3 c0 = _my->transform()->getPosition();
		glm::vec3 n = _other->getNorm();
		n = roundNormals(n);
		glm::vec3 q = _other->transform()->getPosition();
		glm::vec3 scale = _my->transform()->getScale();
		glm::vec3 ci;
		glm::vec3 permCP = rb->getPermCP();
		float dis2Plane = distanceToPlane(n, c0, q);
		float dis2Plane2 = distanceToPlane(n, _c1, q);
		glm::vec3 edgePoint = c0 + scale / 2.0f;
		float dEdge = glm::distance(edgePoint * n, c0 * n);
		glm::vec3 pLength = _other->transform()->getScale() / 2.0f;

		float a, b, c;
		a = b = c = 0.0f;
		a = 1.0f - fabs(n.x);
		b = 1.0f - fabs(n.y);
		c = 1.0f - fabs(n.z);
		glm::vec3 invNormal = glm::vec3(a, b, c);
		glm::vec3 aPos = c0 * invNormal;
		glm::vec3 aLength = pLength * invNormal;
		glm::vec3 amLength = (scale * invNormal) / 2.0f;
		glm::vec3 aPpos = q * invNormal;

		if (fabs(dis2Plane) <= dEdge) //If already colliding with the plane
		{

			if (fabs(aPos.x - amLength.x) > aPpos.x + aLength.x || fabs(aPos.y - amLength.y) > aPpos.y + aLength.y || fabs(aPos.z - amLength.z) > aPpos.z + aLength.z) // If we fall off the plane
			{
				return false;
			}

			if (permCP == glm::vec3(0.0f, 0.0f, 0.0f))
			{
				permCP = q + (dEdge - 0.01f) * n;
			}

			glm::vec3 dif = c0 - permCP;
			dif *= n;
			ci = c0 - dif;
			rb->setFloored(true);
			rb->m_collisions.push_back(std::make_shared<Collision>(ci, n, _other->getEntity(), _my->getEntity()));
		}
		return true;


		if (fabs(dis2Plane) > dEdge && fabs(dis2Plane2) < dEdge) //If colliding with the plane during movement between two timesteps
		{
			if (fabs(aPos.x - amLength.x) > aPpos.x + aLength.x || fabs(aPos.y - amLength.y) > aPpos.y + aLength.y || fabs(aPos.z - amLength.z) > aPpos.z + aLength.z) // If we fall off the plane
			{
				return false;
			}

			float t = (dis2Plane - dEdge) / (dis2Plane - dis2Plane2);
			ci = (1.0f - t) * c0 + t * _c1;
			if (dis2Plane < 0.0f) // If under the plane
			{
				ci *= -1.0f;
			}
			rb->setPermCP(ci);
			rb->m_collisions.push_back(std::make_shared<Collision>(ci, n, _other->getEntity(), _my->getEntity()));
			rb->setFloored(true);
			return true;
		}
		return false;
	}

	//Works, although buggy. DOES NOT WORK WITH 2D PLANES. (But is more than happy with an infinitely thin box)
	bool PhysicsEventUser::meshToMesh(std::shared_ptr<MeshCollider> _my, std::shared_ptr<Collider> _other, glm::vec3 _c1)
	{
		std::shared_ptr<SphereCollider> s = std::static_pointer_cast<SphereCollider>(_other);
		if (s) // Too many polygons. Not worth it
		{
			return false;
		}
		glm::vec3 otherP = _other->transform()->getPosition();
		glm::vec3 pos = _my->transform()->getPosition();
		glm::vec3 length = ((_my->transform()->getScale() / 2.0f) * _my->transform()->getSize()); // We factor size into the calculations because meshes use the same collision function
		glm::vec3 oLength = ((_other->transform()->getScale() / 2.0f) * (_other->transform()->getSize()));



		if (pos.x + length.x >= otherP.x - oLength.x && pos.x - length.x <= otherP.x + oLength.x)
		{
			if (pos.y + length.y >= otherP.y - oLength.y && pos.y - length.y <= otherP.y + oLength.y) //Spatial partitioning using AABB
			{
				if (pos.z + length.z >= otherP.z - oLength.z && pos.z - length.z <= otherP.z + oLength.z)
				{
					std::shared_ptr<PhysicsEventUser> rb = _my->getEntity()->getComponent<PhysicsEventUser>();
					std::shared_ptr<MeshCollider> m = std::static_pointer_cast<MeshCollider>(_other);
					if (m)
					{
						if (m->isEasy()) //If a mesh is 'easy' 
						{
							if (fabs(glm::distance(_my->transform()->getPosition(), _other->transform()->getPosition()) <= _other->transform()->getScale().y * _other->transform()->getSize().y)) //Treat as a sphere 
							{
								return true;
							}
							else { return false; }
						}
					}


					// Their hitboxes overlap, now test the actual triangle affected
					VertexBuffer *otherTriPos = _other->getEntity()->getComponent<MeshRenderer>()->m_vAO->getTriPos();
					VertexBuffer *myTriPos = _my->getEntity()->getComponent<MeshRenderer>()->m_vAO->getTriPos();
					glm::mat4 myModel = _my->transform()->getModel();
					glm::mat4 oModel = _other->transform()->getModel();
					glm::vec3 cN;
					glm::vec3 cP;
					bool collideMesh = false;
					glm::vec3 offset = glm::vec3(0.0f);
					int collideEarly = 0;

					if (_other->getEntity()->getTag() == _my->getLastCol() && _my->colBefore()) // If we're colliding with the same mesh as our last collision, check for an 'early exit'.
					{
						//If the same two triangles as last time collide, we can skip checking them all
						std::vector<int> otherLtri = _my->getColTri();
						float eo1 = otherTriPos->getData(otherLtri[0]);
						float eo2 = otherTriPos->getData(otherLtri[1]);
						float eo3 = otherTriPos->getData(otherLtri[2]);
						float eo4 = otherTriPos->getData(otherLtri[3]);
						float eo5 = otherTriPos->getData(otherLtri[4]);
						float eo6 = otherTriPos->getData(otherLtri[5]);
						float eo7 = otherTriPos->getData(otherLtri[6]);
						float eo8 = otherTriPos->getData(otherLtri[7]);
						float eo9 = otherTriPos->getData(otherLtri[8]);

						glm::vec4 vEo1 = glm::vec4(eo1, eo2, eo3, 1.0f);
						glm::vec4 vEo2 = glm::vec4(eo4, eo5, eo6, 1.0f);
						glm::vec4 vEo3 = glm::vec4(eo7, eo8, eo9, 1.0f);

						vEo1 = oModel * vEo1;
						float eEo1[3] = { vEo1.x, vEo1.y, vEo1.z };

						vEo2 = oModel * vEo2;
						float eEo2[3] = { vEo2.x, vEo2.y, vEo2.z };

						vEo3 = oModel * vEo3;
						float eEo3[3] = { vEo3.x, vEo3.y, vEo3.z };

						cN = glm::normalize(glm::cross(glm::vec3(eEo1[0], eEo1[1], eEo1[2]), glm::vec3(eEo2[0], eEo2[1], eEo2[2])));
						cN = roundNormals(cN);

						std::vector<int> myTri = _my->getMyTri();
						float e1 = myTriPos->getData(myTri[0]);
						float e2 = myTriPos->getData(myTri[1]);
						float e3 = myTriPos->getData(myTri[2]);
						float e4 = myTriPos->getData(myTri[3]);
						float e5 = myTriPos->getData(myTri[4]);
						float e6 = myTriPos->getData(myTri[5]);
						float e7 = myTriPos->getData(myTri[6]);
						float e8 = myTriPos->getData(myTri[7]);
						float e9 = myTriPos->getData(myTri[8]);

						glm::vec4 vE1 = glm::vec4(e1, e2, e3, 1.0f);
						glm::vec4 vE2 = glm::vec4(e4, e5, e6, 1.0f);
						glm::vec4 vE3 = glm::vec4(e7, e8, e9, 1.0f);

						vE1 = myModel * vE1;
						float eE1[3] = { vE1.x, vE1.y, vE1.z };

						vE2 = myModel * vE2;
						float eE2[3] = { vE2.x, vE2.y, vE2.z };

						vE3 = myModel * vE3;
						float eE3[3] = { vE3.x, vE3.y, vE3.z };

						

						collideEarly = NoDivTriTriIsect(eE1, eE2, eE3, eEo1, eEo2, eEo3); //Compare the last two known triangles to see if they're still colliding
						if (collideEarly)
						{
							float normal[3] = { cN.x, cN.y, cN.z };
							int steps = -1;			//This is -1 because we want to use number of steps -1 as the final multiply value.	
							float precision = _my->getPrecision();
							while (NoDivTriTriIsect(eE1, eE2, eE3, eEo1, eEo2, eEo3)) // Keep moving until we don't collide
							{
								steps++;
								for (int v = 0; v < 3; v++)
								{
									eE1[v] += precision * normal[v]; // Move all of the points 'precision' distance away in the 'normal' direction
								}
								for (int v = 0; v < 3; v++)
								{
									eE2[v] += precision * normal[v];
								}
								for (int v = 0; v < 3; v++)
								{
									eE3[v] += precision * normal[v];
								}
								if (steps > 2000)
								{
									steps = 2000;
									break;
								} //Avoid un-ending loops because the object can't budge for some reason
							}
							offset += (precision * steps) * cN;

							if (cN == glm::vec3(0.0f, 1.0f, 0.0f)) // We're currently on and colliding with a floor
							{
								rb->setFloored(true); //Saves so much effort
							}

							if (rb->isFloored() && offset.y < 0.0f) // If we're on a floor, stay on it.
							{
								offset.y = 0.0f;
							}	
							cP = _my->transform()->getPosition() + offset;  // Set the CP to the last place we collided at.

							rb->m_collisions.push_back(std::make_shared<Collision>(cP, cN, _other->getEntity(), _my->getEntity()));
							return true;
						}
					}

					if (collideEarly == false) //We didn't collide early, time to check them all
					{

						int partition1 = 9;
						if (otherTriPos->getDataSize() > 1500)
						{
							partition1 *= 2;
						}

						if (otherTriPos->getDataSize() > 3000)
						{
							partition1 *= 2;
						}

						if (otherTriPos->getDataSize() > 6000)
						{
							partition1 *= 2;
						}

						if (otherTriPos->getDataSize() > 12000)
						{
							partition1 *= 2;
						}
						if (otherTriPos->getDataSize() > 24000) //If it's a big polygon, we can check 1 in every x triangles
						{
							partition1 *= 2;
						}

						for (int o = 0; o < otherTriPos->getDataSize() - partition1; o += partition1)
						{

							float u1x = otherTriPos->getData(o);
							float u1y = otherTriPos->getData(o + 1);
							float u1z = otherTriPos->getData(o + 2);

							float u2x = otherTriPos->getData(o + 3);
							float u2y = otherTriPos->getData(o + 4);
							float u2z = otherTriPos->getData(o + 5);

							float u3x = otherTriPos->getData(o + 6);
							float u3y = otherTriPos->getData(o + 7);
							float u3z = otherTriPos->getData(o + 8);


							glm::vec4 vU1 = glm::vec4(u1x, u1y, u1z, 1.0f);
							glm::vec4 vU2 = glm::vec4(u2x, u2y, u2z, 1.0f);
							glm::vec4 vU3 = glm::vec4(u3x, u3y, u3z, 1.0f);

							vU1 = oModel * vU1;
							float u1[3] = { vU1.x, vU1.y, vU1.z };

							vU2 = oModel * vU2;
							float u2[3] = { vU2.x, vU2.y, vU2.z };

							vU3 = oModel * vU3;
							float u3[3] = { vU3.x, vU3.y, vU3.z };

							glm::vec3 p1, p2, p3;
							p1 = glm::vec3(u1[0], u1[1], u1[2]);
							p2 = glm::vec3(u2[0], u2[1], u2[2]);
							p3 = glm::vec3(u3[0], u3[1], u3[2]);
							cN = glm::normalize(glm::cross(p2 - p1, p3 - p1));


							float a = (u1[0] + u2[0] + u3[0]) / 3.0f;
							float b = (u1[1] + u2[1] + u3[1]) / 3.0f;
							float c = (u1[2] + u2[2] + u3[2]) / 3.0f;

							glm::vec3 triLoc = glm::vec3(a, b, c);

							int partition2 = 9;
							if (myTriPos->getDataSize() > 1500)
							{
								partition2 *= 2;
							}

							if (myTriPos->getDataSize() > 3000)
							{
								partition2 *= 2;
							}
							if (myTriPos->getDataSize() > 6000)
							{
								partition2 *= 2;
							}
							if (myTriPos->getDataSize() > 12000)
							{
								partition2 *= 2;
							}


							for (int l = 0; l < myTriPos->getDataSize() - partition2; l += partition2)
							{

								float v1x = myTriPos->getData(l);
								float v1y = myTriPos->getData(l + 1);
								float v1z = myTriPos->getData(l + 2);

								float v2x = myTriPos->getData(l + 3);
								float v2y = myTriPos->getData(l + 4);
								float v2z = myTriPos->getData(l + 5);

								float v3x = myTriPos->getData(l + 6);
								float v3y = myTriPos->getData(l + 7);
								float v3z = myTriPos->getData(l + 8);


								glm::vec4 vV1 = glm::vec4(v1x, v1y, v1z, 1.0f);
								glm::vec4 vV2 = glm::vec4(v2x, v2y, v2z, 1.0f);
								glm::vec4 vV3 = glm::vec4(v3x, v3y, v3z, 1.0f);

								vV1 = myModel * vV1;
								//vV1 += glm::vec4(offset, 0.0f);
								float v1[3] = { vV1.x, vV1.y, vV1.z };

								vV2 = myModel * vV2;
								//vV2 += glm::vec4(offset, 0.0f);
								float v2[3] = { vV2.x, vV2.y, vV2.z };

								vV3 = myModel * vV3;
								//vV3 += glm::vec4(offset, 0.0f);
								float v3[3] = { vV3.x, vV3.y, vV3.z };


								cN = roundNormals(cN);
								int collideTri = 0;
								glm::vec3 rayToTri = _my->transform()->getPosition() - triLoc; //Cast a ray from position to the location of the triangle

								if (glm::dot(cN, -rayToTri) < 0.0f - 0.0001f) //refuse to register collision with a triangle that faces away from us
								{
									collideTri = NoDivTriTriIsect(v1, v2, v3, u1, u2, u3);
								}

								if (collideTri)
								{
									std::vector<int> colTri; // Store the data positions of the two colliding triangles for faster re-checking later
									colTri.resize(9);
									for (int t = 0; t < 9; t++)
									{
										colTri[t] = o + t;
									}
									std::vector<int> myTri;
									myTri.resize(9);
									for (int t = 0; t < 9; t++)
									{
										myTri[t] = l + t;
									}	

									_my->setColTri(colTri);
									_my->setMyTri(myTri);
									
									float normal[3] = { cN.x, cN.y, cN.z };
									int steps = -1;			//This is -1 because we want to use number of steps -1 as the final multiply value.	
									float precision;									
									precision = _my->getPrecision(); 

									while (NoDivTriTriIsect(v1, v2, v3, u1, u2, u3)) // Keep moving until we don't collide
									{
										steps++;
										for (int v = 0; v < 3; v++)
										{
											v1[v] += precision * normal[v]; // Move all of the points 'precision' distance away in the 'normal' direction
										}
										for (int v = 0; v < 3; v++)
										{
											v2[v] += precision * normal[v];
										}
										for (int v = 0; v < 3; v++)
										{
											v3[v] += precision * normal[v];
										}
										if (steps > 2000)
										{
											steps = 2000;
											break;
										} //Avoid un-ending loops because the object can't budge for some reason
									}


									if (cN == glm::vec3(0.0f, 1.0f, 0.0f)) // We're currently on and colliding with a floor
									{
										rb->setFloored(true); //Saves so much effort
									}


									offset += (precision * steps) * cN;									
									if (rb->isFloored() && offset.y < 0.0f) // If we're on a floor, stay on it.
									{
										offset.y = 0.0f;
									}
									
									collideMesh = true;
								}
							}

							if (collideMesh)
							{
								cP = _my->transform()->getPosition() + offset;  // Set the CP to the last place we collided at.								
								_my->setColBefore(true);
								_my->setLastCol(_other->getEntity()->getTag());
								rb->m_collisions.push_back(std::make_shared<Collision>(cP, cN, _other->getEntity(), _my->getEntity()));
								return true;
							}
						}
					}
				}
				else { return false; }
			}
			else { return false; }
		}
		else { return false; }
	}

	bool PhysicsEventUser::sphereBoxOverlap(std::shared_ptr<SphereCollider> _sphere, std::shared_ptr<BoxCollider> _box) //https://studiofreya.com/3d-math-and-physics/sphere-vs-aabb-collision-detection-test/
	{
		float dSq = 0.0f;
		glm::vec3 pos = _sphere->transform()->getPosition();
		glm::vec3 bPos = _box->transform()->getPosition();
		glm::vec3 length = _box->transform()->getScale() / 2.0f;

		if (pos.x < bPos.x - length.x)
		{
			dSq += pow((bPos.x - length.x) - pos.x, 2.0f);
		}
		else if (pos.x > bPos.x + length.x)
		{
			dSq += pow(pos.x - (bPos.x + length.x), 2.0f);
		}

		if (pos.y < bPos.y - length.y)
		{
			dSq += pow((bPos.y - length.y) - pos.y, 2.0f);
		}
		else if (pos.y > bPos.y + length.y)
		{
			dSq += pow(pos.y - (bPos.y + length.y), 2.0f);
		}

		if (pos.z < bPos.z - length.z)
		{
			dSq += pow((bPos.z - length.z) - pos.z, 2.0f);
		}
		else if (pos.z > bPos.z + length.z)
		{
			dSq += pow(pos.z - (bPos.z + length.z), 2.0f);
		}

		if (dSq <= pow(_sphere->getRadius(), 2.0f))
		{
			return true;
		}
		else { return false; }
	}

	void PhysicsEventUser::resetCollisions()
	{
		m_collisions.clear();
	}

	void PhysicsObject::onTick()
	{
		glm::vec3 nextPos; // A 'projected' position for next frame. I intent to remove this in favour of better methods.
						   //You'll notice half the functions already ignore this

		glm::vec3 vel = getVelocity();
		if (isFloored() == false)
		{
			float gravity = -9.81f * getMass();
			addForce(glm::vec3(0.0f, gravity, 0.0f)); //Apply force due to gravity		
		}

		else //Another convenient optimisation that saves calculations and reduces 'jitter'
		{
			if (WITHIN(vel.y, 0.0f, 1.0f))
			{
				vel.y = 0.0f;
				setVelocity(vel);
			}
		}
		float _dT = getEntity()->getCore()->getDeltaTime();
		glm::vec3 move = rungeKutta2(_dT, getMass()); //The displacement due to velocity
		if (isFloored() && move.y < 0.0f)
		{
			move.y = 0.0f; //Prevents glitching through the floor
		}

		std::shared_ptr<Transform> trans = getEntity()->getComponent<Transform>();
		trans->setPosition(trans->getPosition() + move); // Move via Runge Kutta 2		
		
		setCollided(false); //We do this here instead of in the 'Reset collision function' because the rotational update needs to know
		nextPos = trans->getPosition() + move;

		bool collided = checkForCollisions(_dT, getEntity()->getCore()->getAllEntities(), nextPos); // Check Collisions

		//if (collided && _my->getShape() == "mesh")//Stop meshes from colliding
		//{
			//_my->setPosition(_my->transform()->getPosition() - move);
			// Go back. Mesh collision is imperfect, but this alongside the 'offset' value used in the intersection...
			//...test helps keep meshes stable and unable to walk through solid objects
		//}
		
		clearForces();
	}

	void AdvPhysicsObject::onTick()
	{
		glm::vec3 nextPos; // A 'projected' position for next frame. I intent to remove this in favour of better methods.
						   //You'll notice half the functions already ignore this

		glm::vec3 vel = getVelocity();


		if (isFloored() == false)
		{
			float gravity = -9.81f * getMass();
			addForce(glm::vec3(0.0f, gravity, 0.0f)); //Apply force due to gravity		
		}

		else //Another convenient optimisation that saves calculations and reduces 'jitter'
		{
			if (WITHIN(vel.y, 0.0f, 1.0f))
			{
				vel.y = 0.0f;
				setVelocity(vel);
			}
		}
		float _dT = getEntity()->getCore()->getDeltaTime();
		glm::vec3 move = rungeKutta2(_dT, getMass()); //The displacement due to velocity
		if (isFloored() && move.y < 0.0f)
		{
			move.y = 0.0f; //Prevents glitching through the floor
		}

		std::shared_ptr<Transform> trans = getEntity()->getComponent<Transform>();
		trans->setPosition(trans->getPosition() + move); // Move via Runge Kutta 2		
		updateRotations(_dT, collided());
		setCollided(false); //We do this here instead of in the 'Reset collision function' because the rotational update needs to know
		nextPos = trans->getPosition() + move;

		bool collided = checkForCollisions(_dT, getEntity()->getCore()->getAllEntities(), nextPos); // Check Collisions

		//if (collided && _my->getShape() == "mesh")//Stop meshes from colliding
		//{
			//_my->setPosition(_my->transform()->getPosition() - move);
			// Go back. Mesh collision is imperfect, but this alongside the 'offset' value used in the intersection...
			//...test helps keep meshes stable and unable to walk through solid objects
		//}
		clearTorque();
		clearForces();
	}

	void PhysicsObject::onInitialise(float _mass, float _bounciness)
	{
		m_mass = _mass;
		m_bounciness = _bounciness;
		m_force = glm::vec3(0.0f, 0.0f, 0.0f);
		
		m_floored = false;
	}

	PhysicsObject::PhysicsObject()
	{
		
	}

	glm::vec3 PhysicsObject::rungeKutta2(float _deltaTs, float _mass)
	{
		glm::vec3 a = m_force / _mass;
		glm::vec3 k1 = _deltaTs * a; // Step one, ordinary Euler calculation at t= 0
		a = (m_force + k1 / 2.0f) / _mass;
		glm::vec3 k2 = _deltaTs * a;  // Evaluate again at t= 0.5 before adjusting the velocity
		m_velocity += k2;
		return (m_velocity * _deltaTs);
	}

	void PhysicsObject::setVelocity(glm::vec3 _vel)
	{
		m_velocity = _vel;
	}

	void PhysicsObject::handleCollisions() // Handles multiple collisions at once
	{
		std::vector<std::shared_ptr<Collision>> collision = m_collisions;

		for (int i = 0; i < collision.size(); i++)
		{
			getEntity()->onCollision(collision[i]);


			glm::vec3 move = transform()->getPosition() - collision[i]->m_cP;
			//The amount the object will move away from another object it collides with so as not to clip

			

			//Floored is a simple boolean optimisation that is used to tell if an object is now resting on a flat surface
			//If it is, we can simplify the responses to save time
			if (isFloored() && move.y > 0.0f)
			{
				move.y = 0.0f;
			}
			transform()->setPosition(transform()->getPosition() - move);

			if (getEntity()->getComponent<MeshCollider>()) // To do: Make velocity in the offending direction turn to 0
			{
				collision[i]->m_deltaVel = glm::vec3(0.0f); // We don't want to adjust Mesh position here. Meshes get weird. Meshes only use pseudo-physics and do not need realistic responses.

			}
			else
			{
				//The friction co-efficient of two objects
				float fricCoEf = collision[i]->m_my->getCollider()->getFriction() + collision[i]->m_other->getCollider()->getFriction() / 10.0f;

				//Also adjust angular velocity by an impulse value if it has advanced physics
				std::shared_ptr<AdvPhysicsObject> aRB = getEntity()->getComponent<AdvPhysicsObject>();
				if (aRB)
				{
					glm::vec3 vel = aRB->getVelocity();
					if (WITHIN(vel.x, 0.0f, 0.01f) && WITHIN(vel.z, 0.0f, 0.01f))
					{
						vel.y = 0.0f;
					}

					if (WITHIN(vel.y, 0.0f, 0.01f) && WITHIN(vel.z, 0.0f, 0.01f))
					{
						vel.x = 0.0f;
					}

					if (WITHIN(vel.x, 0.0f, 0.01f) && WITHIN(vel.y, 0.0f, 0.01f))
					{
						vel.z = 0.0f;
					}

					//If the result force gets too low, it starts to stop rolling prematurely
					float rForce = collision[i]->m_resultForce;
					if (rForce < 0.8f) { rForce = 0.8f; }

					//Angular force
					glm::vec3 aForce = (glm::cross(vel, collision[i]->m_normal) * rForce) * (fricCoEf * (aRB->getMass() / -9.80f));
					aForce *= -9.0f; //Scale the force to make it look realistic

									 //Torque = Sum of the angular foces acting on an object
					aRB->addTorque(aForce);
				}

				glm::vec3 friction = getVelocity() / (fricCoEf * 100.0f);
				setVelocity((getVelocity() + collision[i]->m_deltaVel) - friction);
			}
		}

		//Clears the collisions ready to re-detect next frame
		resetCollisions();
	}

	void AdvPhysicsObject::onInitialise(float _mass, float _bounciness)
	{
		m_torque = glm::vec3(0.0f, 0.0f, 0.0f);
		m_aMom = glm::vec3(0.0f, 0.0f, 0.0f);
		m_rotMat = glm::mat3(1.0f);
		m_rotQuat = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
		m_rotQuat = glm::rotate(m_rotQuat, 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));

		m_mass = _mass;
		m_bounciness = _bounciness;
		m_force = glm::vec3(0.0f, 0.0f, 0.0f);		
		m_floored = false;

		m_orient = glm::vec3(0.0f, 0.0f, 0.0f);

		//A sphere has a body inertia tensor of 2/5 * mass * r^2

		if (getEntity()->getComponent<SphereCollider>())
		{
			m_invBodyInertiaTensor = glm::mat3((2.0f / 5.0f) * _mass * pow(transform()->getScale().y, 2));
		}


		else if (getEntity()->getComponent<BoxCollider>())
		{
			glm::vec3 sizeS = transform()->getScale() / 2.0f;
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
		
	glm::mat3 AdvPhysicsObject::calcInvTensor()
	{
		return (m_rotMat * m_invBodyInertiaTensor * glm::transpose(m_rotMat));
	}

	void AdvPhysicsObject::updateRotations(float _dTs, bool _collided)
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

	std::shared_ptr<RayCollision> RayCaster::rayToTri(std::vector<std::shared_ptr<Entity>> _obj, glm::vec3 _rayDir, glm::vec3 _origin, std::string _rayTag)
	{
		std::shared_ptr<Entity> winObject = nullptr;
		glm::vec3 triLoc = glm::vec3(1000.0f, 1000.0f, 1000.0f); //Random big starting number
		glm::vec3 winTri; //The triangle we hit's position
		float shortest = 1000.0f; //Closest triangle
		bool collideMesh = false;
		VertexBuffer *otherTriPos;
		glm::mat4 oModel;

		for (int i = 0; i < _obj.size(); i++)
		{
			otherTriPos = _obj[i]->getComponent<MeshRenderer>()->m_vAO->getTriPos();
			oModel = _obj[i]->transform()->getModel();
			glm::vec3 tN;

			double* t = new double(0.0);
			double* u = new double(0.0);
			double* v = new double(0.0);

			for (int o = 0; o < otherTriPos->getDataSize() - 9; o += 9)
			{

				float u1x = otherTriPos->getData(o);
				float u1y = otherTriPos->getData(o + 1);
				float u1z = otherTriPos->getData(o + 2);

				float u2x = otherTriPos->getData(o + 3);
				float u2y = otherTriPos->getData(o + 4);
				float u2z = otherTriPos->getData(o + 5);

				float u3x = otherTriPos->getData(o + 6);
				float u3y = otherTriPos->getData(o + 7);
				float u3z = otherTriPos->getData(o + 8);


				glm::vec4 vU1 = glm::vec4(u1x, u1y, u1z, 1.0f);
				glm::vec4 vU2 = glm::vec4(u2x, u2y, u2z, 1.0f);
				glm::vec4 vU3 = glm::vec4(u3x, u3y, u3z, 1.0f);

				vU1 = oModel * vU1;
				double u1[3] = { vU1.x, vU1.y, vU1.z };

				vU2 = oModel * vU2;
				double u2[3] = { vU2.x, vU2.y, vU2.z };

				vU3 = oModel * vU3;
				double u3[3] = { vU3.x, vU3.y, vU3.z };

				glm::vec3 p1, p2, p3;
				p1 = glm::vec3(u1[0], u1[1], u1[2]);
				p2 = glm::vec3(u2[0], u2[1], u2[2]);
				p3 = glm::vec3(u3[0], u3[1], u3[2]);
				tN = glm::normalize(glm::cross(p2 - p1, p3 - p1));


				float a = (u1[0] + u2[0] + u3[0]) / 3.0f;
				float b = (u1[1] + u2[1] + u3[1]) / 3.0f;
				float c = (u1[2] + u2[2] + u3[2]) / 3.0f;
				double orig[3] = { _origin.x, _origin.y, _origin.z };
				double dir[3] = { _rayDir.x, _rayDir.y, _rayDir.z };
				triLoc = glm::vec3(a, b, c);
				glm::vec3 rayToTri = glm::normalize(triLoc - _origin);

				if (glm::dot(-rayToTri, _rayDir) < 0.0f - 0.0001f) //refuse to register collision with a triangle that we're not looking at
				{
					if (intersect_triangle3(orig, dir, u1, u2, u3, t, u, v))
					{
						collideMesh = true;
						float distance = fabs(glm::distance(_origin, triLoc));
						if (distance < shortest)
						{
							shortest = distance;
							winObject = _obj[i];
							winTri = triLoc;
							break;
						}
					}
				}
			}
		}
		std::shared_ptr<RayCollision> col = std::make_shared<RayCollision>();
		col->m_hit = collideMesh;
		if (collideMesh)
		{
			col->m_hitObj = winObject;
			col->m_rayDir = _rayDir;
			col->m_origin = _origin;
			col->m_rayTag = _rayTag;
			col->m_hitPoint = winTri;
		}
		return col;
	}

#pragma region THOMAS_MOLLER_CODE



	/* Triangle/triangle intersection test routine,
	* by Tomas Moller, 1997.
	* See article "A Fast Triangle-Triangle Intersection Test",
	* Journal of Graphics Tools, 2(2), 1997
	*
	* Updated June 1999: removed the divisions -- a little faster now!
	* Updated October 1999: added {} to CROSS and SUB macros
	*
	* int NoDivTriTriIsect(float V0[3],float V1[3],float V2[3],
	*                      float U0[3],float U1[3],float U2[3])
	*
	* parameters: vertices of triangle 1: V0,V1,V2
	*             vertices of triangle 2: U0,U1,U2
	* result    : returns 1 if the triangles intersect, otherwise 0
	*
	*/

#include <math.h>
#define FABS(x) (float(fabs(x)))        /* implement as is fastest on your machine */

	/* if USE_EPSILON_TEST is true then we do a check:
	if |dv|<EPSILON then dv=0.0;
	else no check is done (which is less robust)
	*/
#define USE_EPSILON_TEST TRUE
#define EPSILON 0.000001


	/* some macros */
#define CROSS(dest,v1,v2){                     \
              dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
              dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
              dest[2]=v1[0]*v2[1]-v1[1]*v2[0];}

#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])

#define SUB(dest,v1,v2){         \
            dest[0]=v1[0]-v2[0]; \
            dest[1]=v1[1]-v2[1]; \
            dest[2]=v1[2]-v2[2];}

/* sort so that a<=b */
#define SORT(a,b)       \
             if(a>b)    \
             {          \
               float c; \
               c=a;     \
               a=b;     \
               b=c;     \
             }


/* this edge to edge test is based on Franlin Antonio's gem:
"Faster Line Segment Intersection", in Graphics Gems III,
pp. 199-202 */
#define EDGE_EDGE_TEST(V0,U0,U1)                      \
  Bx=U0[i0]-U1[i0];                                   \
  By=U0[i1]-U1[i1];                                   \
  Cx=V0[i0]-U0[i0];                                   \
  Cy=V0[i1]-U0[i1];                                   \
  f=Ay*Bx-Ax*By;                                      \
  d=By*Cx-Bx*Cy;                                      \
  if((f>0 && d>=0 && d<=f) || (f<0 && d<=0 && d>=f))  \
  {                                                   \
    e=Ax*Cy-Ay*Cx;                                    \
    if(f>0)                                           \
    {                                                 \
      if(e>=0 && e<=f) return 1;                      \
    }                                                 \
    else                                              \
    {                                                 \
      if(e<=0 && e>=f) return 1;                      \
    }                                                 \
  }

#define EDGE_AGAINST_TRI_EDGES(V0,V1,U0,U1,U2) \
{                                              \
  float Ax,Ay,Bx,By,Cx,Cy,e,d,f;               \
  Ax=V1[i0]-V0[i0];                            \
  Ay=V1[i1]-V0[i1];                            \
  /* test edge U0,U1 against V0,V1 */          \
  EDGE_EDGE_TEST(V0,U0,U1);                    \
  /* test edge U1,U2 against V0,V1 */          \
  EDGE_EDGE_TEST(V0,U1,U2);                    \
  /* test edge U2,U1 against V0,V1 */          \
  EDGE_EDGE_TEST(V0,U2,U0);                    \
}

#define POINT_IN_TRI(V0,U0,U1,U2)           \
{                                           \
  float a,b,c,d0,d1,d2;                     \
  /* is T1 completly inside T2? */          \
  /* check if V0 is inside tri(U0,U1,U2) */ \
  a=U1[i1]-U0[i1];                          \
  b=-(U1[i0]-U0[i0]);                       \
  c=-a*U0[i0]-b*U0[i1];                     \
  d0=a*V0[i0]+b*V0[i1]+c;                   \
                                            \
  a=U2[i1]-U1[i1];                          \
  b=-(U2[i0]-U1[i0]);                       \
  c=-a*U1[i0]-b*U1[i1];                     \
  d1=a*V0[i0]+b*V0[i1]+c;                   \
                                            \
  a=U0[i1]-U2[i1];                          \
  b=-(U0[i0]-U2[i0]);                       \
  c=-a*U2[i0]-b*U2[i1];                     \
  d2=a*V0[i0]+b*V0[i1]+c;                   \
  if(d0*d1>0.0)                             \
  {                                         \
    if(d0*d2>0.0) return 1;                 \
  }                                         \
}

	int PhysicsEventUser::coplanar_tri_tri(float N[3], float V0[3], float V1[3], float V2[3],
		float U0[3], float U1[3], float U2[3])
	{
		float A[3];
		short i0, i1;
		/* first project onto an axis-aligned plane, that maximizes the area */
		/* of the triangles, compute indices: i0,i1. */
		A[0] = FABS(N[0]);
		A[1] = FABS(N[1]);
		A[2] = FABS(N[2]);
		if (A[0] > A[1])
		{
			if (A[0] > A[2])
			{
				i0 = 1;      /* A[0] is greatest */
				i1 = 2;
			}
			else
			{
				i0 = 0;      /* A[2] is greatest */
				i1 = 1;
			}
		}
		else   /* A[0]<=A[1] */
		{
			if (A[2] > A[1])
			{
				i0 = 0;      /* A[2] is greatest */
				i1 = 1;
			}
			else
			{
				i0 = 0;      /* A[1] is greatest */
				i1 = 2;
			}
		}

		/* test all edges of triangle 1 against the edges of triangle 2 */
		EDGE_AGAINST_TRI_EDGES(V0, V1, U0, U1, U2);
		EDGE_AGAINST_TRI_EDGES(V1, V2, U0, U1, U2);
		EDGE_AGAINST_TRI_EDGES(V2, V0, U0, U1, U2);

		/* finally, test if tri1 is totally contained in tri2 or vice versa */
		POINT_IN_TRI(V0, U0, U1, U2);
		POINT_IN_TRI(U0, V0, V1, V2);

		return 0;
	}



#define NEWCOMPUTE_INTERVALS(VV0,VV1,VV2,D0,D1,D2,D0D1,D0D2,A,B,C,X0,X1) \
{ \
        if(D0D1>0.0f) \
        { \
                /* here we know that D0D2<=0.0 */ \
            /* that is D0, D1 are on the same side, D2 on the other or on the plane */ \
                A=VV2; B=(VV0-VV2)*D2; C=(VV1-VV2)*D2; X0=D2-D0; X1=D2-D1; \
        } \
        else if(D0D2>0.0f)\
        { \
                /* here we know that d0d1<=0.0 */ \
            A=VV1; B=(VV0-VV1)*D1; C=(VV2-VV1)*D1; X0=D1-D0; X1=D1-D2; \
        } \
        else if(D1*D2>0.0f || D0!=0.0f) \
        { \
                /* here we know that d0d1<=0.0 or that D0!=0.0 */ \
                A=VV0; B=(VV1-VV0)*D0; C=(VV2-VV0)*D0; X0=D0-D1; X1=D0-D2; \
        } \
        else if(D1!=0.0f) \
        { \
                A=VV1; B=(VV0-VV1)*D1; C=(VV2-VV1)*D1; X0=D1-D0; X1=D1-D2; \
        } \
        else if(D2!=0.0f) \
        { \
                A=VV2; B=(VV0-VV2)*D2; C=(VV1-VV2)*D2; X0=D2-D0; X1=D2-D1; \
        } \
        else \
        { \
                /* triangles are coplanar */ \
                return coplanar_tri_tri(N1,V0,V1,V2,U0,U1,U2); \
        } \
}

	int PhysicsEventUser::NoDivTriTriIsect(float V0[3], float V1[3], float V2[3],
		float U0[3], float U1[3], float U2[3])
	{
		float E1[3], E2[3];
		float N1[3], N2[3], d1, d2;
		float du0, du1, du2, dv0, dv1, dv2;
		float D[3];
		float isect1[2], isect2[2];
		float du0du1, du0du2, dv0dv1, dv0dv2;
		short index;
		float vp0, vp1, vp2;
		float up0, up1, up2;
		float bb, cc, max;

		/* compute plane equation of triangle(V0,V1,V2) */
		SUB(E1, V1, V0);
		SUB(E2, V2, V0);
		CROSS(N1, E1, E2);
		d1 = -DOT(N1, V0);
		/* plane equation 1: N1.X+d1=0 */

		/* put U0,U1,U2 into plane equation 1 to compute signed distances to the plane*/
		du0 = DOT(N1, U0) + d1;
		du1 = DOT(N1, U1) + d1;
		du2 = DOT(N1, U2) + d1;

		/* coplanarity robustness check */
#if USE_EPSILON_TEST==TRUE
		if (FABS(du0) < EPSILON) du0 = 0.0;
		if (FABS(du1) < EPSILON) du1 = 0.0;
		if (FABS(du2) < EPSILON) du2 = 0.0;
#endif
		du0du1 = du0 * du1;
		du0du2 = du0 * du2;

		if (du0du1 > 0.0f && du0du2 > 0.0f) /* same sign on all of them + not equal 0 ? */
			return 0;                    /* no intersection occurs */

										 /* compute plane of triangle (U0,U1,U2) */
		SUB(E1, U1, U0);
		SUB(E2, U2, U0);
		CROSS(N2, E1, E2);
		d2 = -DOT(N2, U0);
		/* plane equation 2: N2.X+d2=0 */

		/* put V0,V1,V2 into plane equation 2 */
		dv0 = DOT(N2, V0) + d2;
		dv1 = DOT(N2, V1) + d2;
		dv2 = DOT(N2, V2) + d2;

#if USE_EPSILON_TEST==TRUE
		if (FABS(dv0) < EPSILON) dv0 = 0.0;
		if (FABS(dv1) < EPSILON) dv1 = 0.0;
		if (FABS(dv2) < EPSILON) dv2 = 0.0;
#endif

		dv0dv1 = dv0 * dv1;
		dv0dv2 = dv0 * dv2;

		if (dv0dv1 > 0.0f && dv0dv2 > 0.0f) /* same sign on all of them + not equal 0 ? */
			return 0;                    /* no intersection occurs */

										 /* compute direction of intersection line */
		CROSS(D, N1, N2);

		/* compute and index to the largest component of D */
		max = (float)FABS(D[0]);
		index = 0;
		bb = (float)FABS(D[1]);
		cc = (float)FABS(D[2]);
		if (bb > max) max = bb, index = 1;
		if (cc > max) max = cc, index = 2;

		/* this is the simplified projection onto L*/
		vp0 = V0[index];
		vp1 = V1[index];
		vp2 = V2[index];

		up0 = U0[index];
		up1 = U1[index];
		up2 = U2[index];

		/* compute interval for triangle 1 */
		float a, b, c, x0, x1;
		NEWCOMPUTE_INTERVALS(vp0, vp1, vp2, dv0, dv1, dv2, dv0dv1, dv0dv2, a, b, c, x0, x1);

		/* compute interval for triangle 2 */
		float d, e, f, y0, y1;
		NEWCOMPUTE_INTERVALS(up0, up1, up2, du0, du1, du2, du0du1, du0du2, d, e, f, y0, y1);

		float xx, yy, xxyy, tmp;
		xx = x0 * x1;
		yy = y0 * y1;
		xxyy = xx * yy;

		tmp = a * xxyy;
		isect1[0] = tmp + b * x1*yy;
		isect1[1] = tmp + c * x0*yy;

		tmp = d * xxyy;
		isect2[0] = tmp + e * xx*y1;
		isect2[1] = tmp + f * xx*y0;

		SORT(isect1[0], isect1[1]);
		SORT(isect2[0], isect2[1]);

		if (isect1[1] < isect2[0] || isect2[1] < isect1[0]) return 0;
		return 1;
	}


	/********************************************************/
	/* ray-triangle overlap test code                      */
	/* by Tomas Akenine-Möller                              */
	/********************************************************/
	int RayCaster::intersect_triangle3(double orig[3], double dir[3], double vert0[3], double vert1[3], double vert2[3], double * t, double * u, double * v)
	{
		double edge1[3], edge2[3], tvec[3], pvec[3], qvec[3];
		double det, inv_det;

		/* find vectors for two edges sharing vert0 */
		SUB(edge1, vert1, vert0);
		SUB(edge2, vert2, vert0);

		/* begin calculating determinant - also used to calculate U parameter */
		CROSS(pvec, dir, edge2);

		/* if determinant is near zero, ray lies in plane of triangle */
		det = DOT(edge1, pvec);

		/* calculate distance from vert0 to ray origin */
		SUB(tvec, orig, vert0);
		inv_det = 1.0 / det;

		CROSS(qvec, tvec, edge1);

		if (det > EPSILON)
		{
			*u = DOT(tvec, pvec);
			if (*u < 0.0 || *u > det)
				return 0;

			/* calculate V parameter and test bounds */
			*v = DOT(dir, qvec);
			if (*v < 0.0 || *u + *v > det)
				return 0;

		}
		else if (det < -EPSILON)
		{
			/* calculate U parameter and test bounds */
			*u = DOT(tvec, pvec);
			if (*u > 0.0 || *u < det)
				return 0;

			/* calculate V parameter and test bounds */
			*v = DOT(dir, qvec);
			if (*v > 0.0 || *u + *v < det)
				return 0;
		}
		else return 0;  /* ray is parallell to the plane of the triangle */

		*t = DOT(edge2, qvec) * inv_det;
		(*u) *= inv_det;
		(*v) *= inv_det;

		return 1;
	}

	/********************************************************/
	/* AABB-triangle overlap test code                      */
	/* by Tomas Akenine-Möller                              */
	/* Function: int triBoxOverlap(float boxcenter[3],      */
	/*          float boxhalfsize[3],float triverts[3][3]); */
	/* History:                                             */
	/*   2001-03-05: released the code in its first version */
	/*   2001-06-18: changed the order of the tests, faster */
	/*                                                      */
	/* Acknowledgement: Many thanks to Pierre Terdiman for  */
	/* suggestions and discussions on how to optimize code. */
	/* Thanks to David Hunt for finding a ">="-bug!         */
	/********************************************************/


#define X 0
#define Y 1
#define Z 2

#define CROSS(dest,v1,v2) \
          dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
          dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
          dest[2]=v1[0]*v2[1]-v1[1]*v2[0];

#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])

#define SUB(dest,v1,v2) \
          dest[0]=v1[0]-v2[0]; \
          dest[1]=v1[1]-v2[1]; \
          dest[2]=v1[2]-v2[2];

#define FINDMINMAX(x0,x1,x2,min,max) \
  min = max = x0;   \
  if(x1<min) min=x1;\
  if(x1>max) max=x1;\
  if(x2<min) min=x2;\
  if(x2>max) max=x2;

	int PhysicsEventUser::planeBoxOverlap(float normal[3], float d, float maxbox[3])
	{
		int q;
		float vmin[3], vmax[3];
		for (q = X; q <= Z; q++)
		{
			if (normal[q] > 0.0f)
			{
				vmin[q] = -maxbox[q];
				vmax[q] = maxbox[q];
			}
			else
			{
				vmin[q] = maxbox[q];
				vmax[q] = -maxbox[q];
			}
		}
		if (DOT(normal, vmin) + d > 0.0f) return 0;
		if (DOT(normal, vmax) + d >= 0.0f) return 1;

		return 0;
	}


	/*======================== X-tests ========================*/
#define AXISTEST_X01(a, b, fa, fb)             \
    p0 = a*v0[Y] - b*v0[Z];                    \
    p2 = a*v2[Y] - b*v2[Z];                    \
        if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;} \
    rad = fa * boxhalfsize[Y] + fb * boxhalfsize[Z];   \
    if(min>rad || max<-rad) return 0;

#define AXISTEST_X2(a, b, fa, fb)              \
    p0 = a*v0[Y] - b*v0[Z];                    \
    p1 = a*v1[Y] - b*v1[Z];                    \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
    rad = fa * boxhalfsize[Y] + fb * boxhalfsize[Z];   \
    if(min>rad || max<-rad) return 0;

/*======================== Y-tests ========================*/
#define AXISTEST_Y02(a, b, fa, fb)             \
    p0 = -a*v0[X] + b*v0[Z];                   \
    p2 = -a*v2[X] + b*v2[Z];                       \
        if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;} \
    rad = fa * boxhalfsize[X] + fb * boxhalfsize[Z];   \
    if(min>rad || max<-rad) return 0;

#define AXISTEST_Y1(a, b, fa, fb)              \
    p0 = -a*v0[X] + b*v0[Z];                   \
    p1 = -a*v1[X] + b*v1[Z];                       \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
    rad = fa * boxhalfsize[X] + fb * boxhalfsize[Z];   \
    if(min>rad || max<-rad) return 0;

/*======================== Z-tests ========================*/

#define AXISTEST_Z12(a, b, fa, fb)             \
    p1 = a*v1[X] - b*v1[Y];                    \
    p2 = a*v2[X] - b*v2[Y];                    \
        if(p2<p1) {min=p2; max=p1;} else {min=p1; max=p2;} \
    rad = fa * boxhalfsize[X] + fb * boxhalfsize[Y];   \
    if(min>rad || max<-rad) return 0;

#define AXISTEST_Z0(a, b, fa, fb)              \
    p0 = a*v0[X] - b*v0[Y];                \
    p1 = a*v1[X] - b*v1[Y];                    \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
    rad = fa * boxhalfsize[X] + fb * boxhalfsize[Y];   \
    if(min>rad || max<-rad) return 0;

	int PhysicsEventUser::triBoxOverlap(float boxcenter[3], float boxhalfsize[3], float triverts[3][3])
	{

		/*    use separating axis theorem to test overlap between triangle and box */
		/*    need to test for overlap in these directions: */
		/*    1) the {x,y,z}-directions (actually, since we use the AABB of the triangle */
		/*       we do not even need to test these) */
		/*    2) normal of the triangle */
		/*    3) crossproduct(edge from tri, {x,y,z}-directin) */
		/*       this gives 3x3=9 more tests */
		float v0[3], v1[3], v2[3];
		float min, max, d, p0, p1, p2, rad, fex, fey, fez;
		float normal[3], e0[3], e1[3], e2[3];

		/* This is the fastest branch on Sun */
		/* move everything so that the boxcenter is in (0,0,0) */
		SUB(v0, triverts[0], boxcenter);
		SUB(v1, triverts[1], boxcenter);
		SUB(v2, triverts[2], boxcenter);

		/* compute triangle edges */
		SUB(e0, v1, v0);      /* tri edge 0 */
		SUB(e1, v2, v1);      /* tri edge 1 */
		SUB(e2, v0, v2);      /* tri edge 2 */

							  /* Bullet 3:  */
							  /*  test the 9 tests first (this was faster) */
		fex = fabs(e0[X]);
		fey = fabs(e0[Y]);
		fez = fabs(e0[Z]);
		AXISTEST_X01(e0[Z], e0[Y], fez, fey);
		AXISTEST_Y02(e0[Z], e0[X], fez, fex);
		AXISTEST_Z12(e0[Y], e0[X], fey, fex);

		fex = fabs(e1[X]);
		fey = fabs(e1[Y]);
		fez = fabs(e1[Z]);
		AXISTEST_X01(e1[Z], e1[Y], fez, fey);
		AXISTEST_Y02(e1[Z], e1[X], fez, fex);
		AXISTEST_Z0(e1[Y], e1[X], fey, fex);

		fex = fabs(e2[X]);
		fey = fabs(e2[Y]);
		fez = fabs(e2[Z]);
		AXISTEST_X2(e2[Z], e2[Y], fez, fey);
		AXISTEST_Y1(e2[Z], e2[X], fez, fex);
		AXISTEST_Z12(e2[Y], e2[X], fey, fex);

		/* Bullet 1: */
		/*  first test overlap in the {x,y,z}-directions */
		/*  find min, max of the triangle each direction, and test for overlap in */
		/*  that direction -- this is equivalent to testing a minimal AABB around */
		/*  the triangle against the AABB */

		/* test in X-direction */
		FINDMINMAX(v0[X], v1[X], v2[X], min, max);
		if (min > boxhalfsize[X] || max < -boxhalfsize[X]) return 0;

		/* test in Y-direction */
		FINDMINMAX(v0[Y], v1[Y], v2[Y], min, max);
		if (min > boxhalfsize[Y] || max < -boxhalfsize[Y]) return 0;

		/* test in Z-direction */
		FINDMINMAX(v0[Z], v1[Z], v2[Z], min, max);
		if (min > boxhalfsize[Z] || max < -boxhalfsize[Z]) return 0;

		/* Bullet 2: */
		/*  test if the box intersects the plane of the triangle */
		/*  compute plane equation of triangle: normal*x+d=0 */
		CROSS(normal, e0, e1);
		d = -DOT(normal, v0);  /* plane eq: normal.x+d=0 */
		if (!planeBoxOverlap(normal, d, boxhalfsize)) return 0;

		return 1;   /* box and triangle overlaps */
	}
#pragma endregion
}