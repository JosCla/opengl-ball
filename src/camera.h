#ifndef CAMERA_H
#define CAMERA_H
// camera.h
// Defines the camera class, used to create proper view matrices for rendering.

#include <glm/glm.hpp> // gl maths
#include <glm/gtc/type_ptr.hpp>

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 4.0f;
const float MOUSE_SENS = 0.3f;
const float ZOOM = 60.0f;
const float SCROLL_SENS = 2.0f;

enum Camera_Direction {
  FORWARD,
  LEFT,
  RIGHT,
  BACKWARD
};

class Camera
{
  public:
    // camera attributes, including position and direction
    glm::vec3 WorldUp;
    glm::vec3 CameraPosition;
    glm::vec3 CameraDirection;
    glm::vec3 CameraRight;
    glm::vec3 CameraUp;
    // camera angle
    float Yaw;
    float Pitch;
    // other movement parameters
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    float ScrollSensitivity;

    Camera(glm::vec3 = glm::vec3(0.0, 0.0, 0.0), glm::vec3 = glm::vec3(0.0, 1.0, 0.0), float = YAW, float = PITCH);
		
    glm::mat4 GetViewMatrix();
    void ProcessMovement(Camera_Direction, float, bool = false);
    void ProcessMouseMovement(float, float, bool = true);
    void ProcessScroll(float);
		
  private:
    // updates camera attributes according to the camera's angles
    void updateCameraAttributes();
};

#endif
