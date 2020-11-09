#ifndef _UTILITY_H_
#define _UTILITY_H_
#include <vector>
#include <string>
#include "Scene.h"

class GameObject;
class Shader;
class SpotLight;
class DirLight;
class PointLight;
class Camera;
//class Scene;
class RayCollision;

namespace Engine
{
	namespace utility
	{
		void onEveryFrame(Scene* _scene, float _dT);
		void update(GameObject* _obj, Shader* _shader, std::vector<GameObject*> _allObj, float _dT);
		void updateLighting(std::vector<SpotLight*> _sLight, std::vector<DirLight*> _dLight, std::vector<PointLight*> _pLight, Shader *_lSh, Shader *_mSh, Shader* _iSh);
		void moveEntity(GameObject* _obj, glm::vec3 _dir, std::vector<GameObject*> _other, float _dTs);
		GameObject* copy(GameObject* _obj);
		float lerp(float _a, float _b, float _f);
		void shootgun(glm::vec3 _dir, std::vector<GameObject*> _obj, glm::vec3 _origin, GameObject* _shooter, Scene* _scene);
		void onRayHit(RayCollision* _col, GameObject* _shooter, Scene* _scene);
		void onRayHit(Camera* _cam);

	}
}


#endif