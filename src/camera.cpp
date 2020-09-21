// camera.cpp

#include <glad/glad.h> // opengl tags
#include <glm/glm.hpp> // gl maths
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// stdlib
#include <cmath> // for sin, cos

// our files
#include "camera.h" // for Camera declaration

Camera::Camera(glm::vec3 position/* = glm::vec3(0.0, 0.0, 0.0)*/, glm::vec3 up/* = glm::vec3(0.0, 1.0, 0.0)*/, float yaw/* = YAW*/, float pitch/* = PITCH*/) : CameraDirection(glm::vec3(0.0, 0.0, -1.0)), MovementSpeed(SPEED), MouseSensitivity(MOUSE_SENS), Zoom(ZOOM), ScrollSensitivity(SCROLL_SENS)
{
	CameraPosition = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	updateCameraAttributes(); 
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(CameraPosition, CameraPosition + CameraDirection, CameraUp);
}

void Camera::ProcessMovement(Camera_Direction direction, float deltaTime, bool stickToXZ)
{
	float velocity = MovementSpeed * deltaTime;
	glm::vec3 ForwardVector = stickToXZ ? glm::normalize(glm::vec3(CameraDirection.x, 0.0, CameraDirection.z)) : CameraDirection;
	glm::vec3 RightVector = stickToXZ ? glm::normalize(glm::vec3(CameraRight.x, 0.0, CameraRight.z)) : CameraRight;
	switch(direction) {
	case FORWARD:
		CameraPosition += ForwardVector * velocity;
		break;
	case LEFT:
		CameraPosition -= RightVector * velocity;
		break;
	case RIGHT:
		CameraPosition += RightVector * velocity;
		break;
	case BACKWARD:
		CameraPosition -= ForwardVector * velocity;
		break;
	}
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainAngles) {
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;
	
	Yaw += xoffset;
	Pitch += yoffset;
	
	if (constrainAngles)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f) 
			Pitch = -89.0f;
		if (Yaw > 180.0f)
			Yaw -= 360.0f;
		if (Yaw < -180.0f)
			Yaw += 360.0f;
	}
	
	updateCameraAttributes();
}

void Camera::ProcessScroll(float yoffset)
{
	Zoom -= yoffset * ScrollSensitivity;
	if (Zoom < 1.0f)
		Zoom = 1.0f;
	if (Zoom > 60.0f)
		Zoom = 60.0f;
}

void Camera::updateCameraAttributes() {
	// updating camera direction vector
	glm::vec3 direction;
	direction.x = std::cos(glm::radians(Yaw)) * std::cos(glm::radians(Pitch));
	direction.y = std::sin(glm::radians(Pitch));
	direction.z = std::sin(glm::radians(Yaw)) * std::cos(glm::radians(Pitch));
	CameraDirection = glm::normalize(direction);
	
	// updating CameraRight and CameraUp
	CameraRight = glm::normalize(glm::cross(CameraDirection, WorldUp));
	CameraUp = glm::normalize(glm::cross(CameraRight, CameraDirection));
}

