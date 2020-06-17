#include "Camera.h"
#include "glm/gtx/transform.hpp"


Camera::Camera()
	:m_Position(glm::vec3(0.0f, 0.0f, 0.0f)),
	m_TargetPos(glm::vec3(0.0f, 0.0f, 0.0f)),
	m_Up(glm::vec3(0.0f, 1.0f, 0.0f)),
	m_Yaw(0.0f),
	m_Pitch(0.0f)
{
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(m_Position, m_TargetPos, m_Up);
}


OrbitCamera::OrbitCamera()
	:m_Radius(10.0f)
{}

void OrbitCamera::setLookAt(const glm::vec3& target)
{
	m_TargetPos = target;
}

void OrbitCamera::setRadius(float radius)
{
	m_Radius = glm::clamp(radius, 2.0f, 80.0f);
}

void OrbitCamera::rotate(float yaw, float pitch)
{
	m_Yaw = glm::radians(yaw);
	m_Pitch = glm::radians(pitch);

	m_Pitch = glm::clamp(m_Pitch, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);

	updateCameraVectors();
}

void OrbitCamera::updateCameraVectors()
{
	m_Position.x = m_TargetPos.x + m_Radius * cosf(m_Pitch) * sinf(m_Yaw);
	m_Position.y = m_TargetPos.y + m_Radius * sinf(m_Pitch);
	m_Position.z = m_TargetPos.z + m_Radius * cosf(m_Pitch) * cosf(m_Yaw);
}
