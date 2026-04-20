#pragma once

#include <glm/fwd.hpp>
#include <glm/glm.hpp>

namespace SimpleRenderer {
class Camera {
public:
  enum Movement { Forward, Backward, Left, Right, Up, Down };

  inline static constexpr glm::vec3 POSITION = glm::vec3(0.0f, 0.0f, 0.0f);
  inline static constexpr glm::vec3 DIRECTION = glm::vec3(0.0f, 0.0f, 1.0f);
  inline static constexpr glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);

  inline static constexpr float YAW = -90.0f;
  inline static constexpr float PITCH = 0.0f;
  inline static constexpr float FOV = 90.0f;

  inline static constexpr int WIDTH = 640;
  inline static constexpr int HEIGHT = 480;
  inline static constexpr float NEAR = 1.0f;
  inline static constexpr float FAR = 100.0f;

  Camera(glm::vec3 position = POSITION, glm::vec3 direction = DIRECTION,
         glm::vec3 worldUp = WORLD_UP,

         float yaw = YAW, float pitch = PITCH, float fov = FOV,

         float width = WIDTH, float height = HEIGHT, float near = NEAR,
         float far = FAR);

  glm::vec3 getPosition() const;
  glm::vec3 getDirection() const;
  glm::vec3 getUp() const;
  glm::vec3 getRight() const;
  glm::vec3 getWorldUp() const;

  float getYaw() const;
  float getPitch() const;
  float getFov() const;

  float getWidth() const;
  float getHeight() const;
  float getNear() const;
  float getFar() const;

  glm::mat4 getViewMatrix() const;
  glm::mat4 getProjectionMatrix() const;

  Camera& setPosition(glm::vec3 position);
  Camera& getDirection(glm::vec3 direction);
  Camera& setWorldUp(glm::vec3 worldUp);

  Camera& setYaw(float yaw);
  Camera& setPitch(float pitch);
  Camera& setFov(float fov);

  Camera& setWidth(float width);
  Camera& setHeight(float height);
  Camera& setNear(float near);
  Camera& setFar(float far);

  void processCameraMovement(Movement movement, double deltaTime);
  void processCameraAngle(glm::vec2 positionOffset);

private:
  glm::vec3 position;
  glm::vec3 direction;
  glm::vec3 up;
  glm::vec3 right;
  glm::vec3 worldUp;

  float yaw;
  float pitch;
  float fov;

  float width;
  float height;
  float near;
  float far;

  void updateCameraVectors();
};
} // namespace SimpleRenderer
