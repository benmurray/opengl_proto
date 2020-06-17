#pragma once
#include "glm/glm.hpp"


class Camera
{
public:
	glm::mat4 getViewMatrix() const;
	virtual void rotate(float yaw, float pitch) {} // in degrees

protected:
	Camera();

	glm::vec3 m_Position;
	glm::vec3 m_TargetPos;
	glm::vec3 m_Up;

	// Euler angles (in radians)
	float m_Yaw;
	float m_Pitch;
};


class OrbitCamera : public Camera
{
public:
	OrbitCamera();

	void rotate(float yaw, float pitch); // in degrees

	void setLookAt(const glm::vec3& target);
	void setRadius(float radius);

private:
	float m_Radius;

	void updateCameraVectors();
};
