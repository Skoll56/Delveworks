#ifndef _CAMERA_H_
#define _CAMERA_H_

namespace Engine
{
	class Input;

	class Camera
	{		
	private:
		glm::vec3 m_pos;
		glm::vec3 m_rot;
		float m_pitch;
		float m_yaw;
		float m_speed;
		glm::mat4 m_model;
		float m_rotSpeed;
		glm::vec3 m_fwd;
		glm::vec3 m_upVec;
		glm::vec3 m_right;

	public:
		Camera(glm::vec3 _startPos);
		~Camera();

		float speed() { return m_speed; }
		glm::vec3 getPos() { return m_pos; }
		glm::vec3 getUp() { return m_upVec; }
		glm::vec3 getRight() { return m_right; }
		float getPitch() { return m_pitch; }
		float getYaw() { return m_yaw; }
		void setPos(glm::vec3 _pos) { m_pos = _pos; }
		void setRot(glm::vec3 _rot) { m_rot = _rot; }
		void update(float _dTime, Input *_input);
		glm::vec3 getFwd() { return m_fwd; }
	};
}

#endif