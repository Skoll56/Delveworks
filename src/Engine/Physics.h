#ifndef _PHYSICS_H_
#define _PHYSICS_H_
#include <vector>
#include <Engine/Engine.h>
namespace Engine
{
	//class GameObject;
	class Collision;
	class RayCollision;
	namespace Physics {

		void update(GameObject* _my, std::vector<GameObject*> _other, float _dT);
		bool collide(float _dT, std::vector<GameObject*> _other, GameObject * _my, glm::vec3 _c1);
		void handleCollisions(GameObject* _object);
		void onCollision(Collision* _col);
		int coplanar_tri_tri(float N[3], float V0[3], float V1[3], float V2[3],
			float U0[3], float U1[3], float U2[3]);

		int NoDivTriTriIsect(float V0[3], float V1[3], float V2[3],
			float U0[3], float U1[3], float U2[3]);

		int triBoxOverlap(float boxcenter[3],
			float boxhalfsize[3], float triverts[3][3]);

		int planeBoxOverlap(float normal[3], float d, float maxbox[3]);

		float distanceToPlane(glm::vec3 &n, glm::vec3 &p, glm::vec3 &q);

		bool boxToBox(GameObject *_my, GameObject* _other, glm::vec3 _c1);

		bool meshToMesh(GameObject *_my, GameObject* _other, glm::vec3 _c1);

		bool sphereToPlane(GameObject *_my, GameObject *_other, glm::vec3 _c1);
		bool boxToPlane(GameObject *_my, GameObject * _other, glm::vec3 _c1);
		bool sphereToBox(GameObject *_my, GameObject *_other, glm::vec3 _c1);
		bool sphereToSphere(GameObject *_my, GameObject *_other);
		bool boxToSphere(GameObject *_my, GameObject *_other, glm::vec3 _c1);
		bool sphereBoxOverlap(GameObject* _sphere, GameObject *_box);
		RayCollision* rayToTri(std::vector<GameObject*> _obj, glm::vec3 _rayDir, glm::vec3 _origin, std::string _rayTag);

		//Reference Tomas Moller
		int intersect_triangle3(double orig[3], double dir[3], double vert0[3], double vert1[3], double vert2[3], double *t, double *u, double *v);
		//T = distance to the triangle
		//U and V are two points inside the triangle

		glm::vec3 roundNormals(glm::vec3 _n);
	}
}

#endif