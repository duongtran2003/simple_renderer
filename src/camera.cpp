#include "camera.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>

namespace SimpleRenderer {
Camera::Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 worldUp,
               float yaw, float pitch, float fov, float width, float height,
               float near, float far) {
  this->position = position;
  this->direction = direction;
  this->worldUp = worldUp;
  this->yaw = yaw;
  this->pitch = pitch;
  this->fov = fov;
  this->width = width;
  this->height = height;
  this->near = near;
  this->far = far;

  updateCameraVectors();
}

void Camera::updateCameraVectors() {
  glm::vec3 calculatedDirection;
  calculatedDirection.x =
      glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
  calculatedDirection.y = glm::sin(glm::radians(pitch));
  calculatedDirection.z =
      glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));

  direction = glm::normalize(calculatedDirection);
  right = glm::normalize(glm::cross(direction, worldUp));
  up = glm::normalize(glm::cross(right, direction));
}

glm::vec3 Camera::getPosition() const { return position; }
glm::vec3 Camera::getDirection() const { return direction; }
glm::vec3 Camera::getUp() const { return up; }
glm::vec3 Camera::getRight() const { return right; }
glm::vec3 Camera::getWorldUp() const { return worldUp; }

float Camera::getYaw() const { return yaw; }
float Camera::getPitch() const { return pitch; }
float Camera::getFov() const { return fov; }
float Camera::getWidth() const { return width; }
float Camera::getHeight() const { return height; }
float Camera::getNear() const { return near; }
float Camera::getFar() const { return far; }

glm::mat4 Camera::getViewMatrix() const {
  return glm::lookAt(position, position + direction, up);
}

glm::mat4 Camera::getProjectionMatrix() const {
  return glm::perspective(glm::radians(fov), width / height, near, far);
}

Camera &Camera::setPosition(glm::vec3 position) {
  this->position = position;
  return *this;
}
Camera &Camera::setDirection(glm::vec3 direction) {
  this->direction = direction;
  updateCameraVectors();
  return *this;
}
Camera &Camera::setWorldUp(glm::vec3 worldUp) {
  this->worldUp = worldUp;
  updateCameraVectors();
  return *this;
}
Camera &Camera::setYaw(float yaw) {
  this->yaw = yaw;
  updateCameraVectors();
  return *this;
}
Camera &Camera::setPitch(float pitch) {
  this->pitch = pitch;
  updateCameraVectors();
  return *this;
}
Camera &Camera::setFov(float fov) {
  this->fov = fov;
  return *this;
}
Camera &Camera::setWidth(float width) {
  this->width = width;
  return *this;
}
Camera &Camera::setHeight(float height) {
  this->height = height;
  return *this;
}
Camera &Camera::setNear(float near) {
  this->near = near;
  return *this;
}
Camera &Camera::setFar(float far) {
  this->far = far;
  return *this;
}

void Camera::processCameraMovement(Movement movement, double deltaTime) {
  float velocity = 5.0f * deltaTime;
  switch (movement) {
  case Forward:
    position += direction * velocity;
    break;
  case Backward:
    position -= direction * velocity;
    break;
  case Left:
    position -= right * velocity;
    break;
  case Right:
    position += right * velocity;
    break;
  case Up:
    position += up * velocity;
    break;
  case Down:
    position -= up * velocity;
    break;
  default:
    break;
  }
}

void Camera::processCameraAngle(glm::vec2 positionOffset, double deltaTime) {
  glm::vec2 sensitivity = glm::vec2(10.0f, 6.0f);
  glm::vec2 offset = glm::vec2(positionOffset.x * sensitivity.x * deltaTime,
                               -positionOffset.y * sensitivity.y * deltaTime);

  yaw += offset.x;
  pitch += offset.y;

  updateCameraVectors();
}
} // namespace SimpleRenderer
