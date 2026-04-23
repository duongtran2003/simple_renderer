#include "app.hpp"
#include "SDL_events.h"
#include "SDL_keycode.h"
#include "SDL_mouse.h"
#include "SDL_stdinc.h"
#include "SDL_timer.h"
#include "camera.hpp"
#include "hello_world_triangle_pipeline.hpp"
#include "input.hpp"
#include "pipeline.hpp"
#include "window.hpp"
#include <cstdint>
#include <cstdio>
#include <ctime>
#include <glm/common.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>
#include <vector>

std::vector<float> getCubeData();
void handleCameraInput(SimpleRenderer::Camera &camera,
                       SimpleRenderer::Input &input, double deltaTime);

namespace SimpleRenderer {

App::App() {
  lastFrameTicks = SDL_GetPerformanceCounter();
  deltaTime = 0.0;
}

void App::calculateDeltaTime() {
  static const double frequency =
      static_cast<double>(SDL_GetPerformanceFrequency());
  uint64_t currentTicks = SDL_GetPerformanceCounter();
  deltaTime = static_cast<double>((currentTicks - lastFrameTicks) / frequency);
  lastFrameTicks = currentTicks;
}

void App::init() {
  const int WINDOW_W = 800;
  const int WINDOW_H = 600;

  Window *window = new Window("My simple renderer", WINDOW_W, WINDOW_H);
  SDL_SetRelativeMouseMode(SDL_TRUE);

  Input &input = Input::getInstance();

  std::vector<float> rawData = getCubeData();

  SDL_Event e;

  HelloWorldTrianglePipeline *helloWorldTrianglePipeline =
      new HelloWorldTrianglePipeline(window->getRenderer());

  helloWorldTrianglePipeline->setDepthTest(true)
      .setDepthMask(true)
      .setEarlyZ(true)
      .setDepthTestFunc(HelloWorldTrianglePipeline::Pipeline::Less)
      .setBufferSize(WINDOW_W, WINDOW_H, window->getRenderer())
      .setClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

  Camera *camera = new Camera();
  camera->setPosition(glm::vec3(0.0f, 0.0f, 3.0f))
      .setDirection(glm::vec3(0.0f, 0.0f, -1.0f))
      .setWorldUp(glm::vec3(0.0f, 1.0f, 0.0f))
      .setYaw(-90.0f)
      .setPitch(0.0f)
      .setFov(75.0f)
      .setWidth(WINDOW_W)
      .setHeight(WINDOW_H)
      .setNear(1.0f)
      .setFar(100.0f);

  float totalTime = 0.0f;

  while (window->isWindowRunning()) {
    calculateDeltaTime();
    while (SDL_PollEvent(&e) != 0) {
      input.processInput(e);
      if (input.getShouldQuit() || input.isKeyPressed(SDLK_ESCAPE)) {
        window->closeWindow();
        return;
      }
    }

    handleCameraInput(*camera, input, deltaTime);

    totalTime += static_cast<float>(deltaTime);

    glm::mat4 view = camera->getViewMatrix();
    glm::mat4 projection = camera->getProjectionMatrix();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
    model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
    model = glm::rotate(model, totalTime, glm::vec3(0.1f, 0.2f, 0.3f));

    helloWorldTrianglePipeline->setUniform("u_viewMatrix", view);
    helloWorldTrianglePipeline->setUniform("u_projectionMatrix", projection);
    helloWorldTrianglePipeline->setUniform("u_modelMatrix", model);

    helloWorldTrianglePipeline->clearColorBuffer();
    helloWorldTrianglePipeline->clearDepthBuffer();
    helloWorldTrianglePipeline->render(rawData);
    window->swapFramebuffer(helloWorldTrianglePipeline->getDrawnFrame());

    input.clearMouseRel();

    printf("Frametime: %.3fms - FPS: %.0f\n", deltaTime, 1.0 / deltaTime);
  }

  return;
}
} // namespace SimpleRenderer

void handleCameraInput(SimpleRenderer::Camera &camera,
                       SimpleRenderer::Input &input, double deltaTime) {
  if (input.isKeyPressed(SDLK_w)) {
    camera.processCameraMovement(SimpleRenderer::Camera::Forward, deltaTime);
  } else if (input.isKeyPressed(SDLK_s)) {
    camera.processCameraMovement(SimpleRenderer::Camera::Backward, deltaTime);
  } else if (input.isKeyPressed(SDLK_a)) {
    camera.processCameraMovement(SimpleRenderer::Camera::Left, deltaTime);
  } else if (input.isKeyPressed(SDLK_d)) {
    camera.processCameraMovement(SimpleRenderer::Camera::Right, deltaTime);
  } else if (input.isKeyPressed(SDLK_SPACE)) {
    camera.processCameraMovement(SimpleRenderer::Camera::Up, deltaTime);
  } else if (input.isKeyPressed(SDLK_LCTRL)) {
    camera.processCameraMovement(SimpleRenderer::Camera::Down, deltaTime);
  }

  glm::vec2 mouseRel = input.getMouseRel();
  if (mouseRel.x != 0.0f || mouseRel.y != 0.0f) {
    camera.processCameraAngle(mouseRel, deltaTime);
  }
}

std::vector<float> getCubeData() {
  return {
      -0.5f, 0.5f,  -0.5f, 1.0f,   0.647f, 0.0f,   1.0f, // A
      -0.5f, 0.5f,  0.5f,  1.0f,   0.647f, 0.0f,   1.0f, // B
      0.5f,  0.5f,  0.5f,  1.0f,   0.647f, 0.0f,   1.0f, // C
      -0.5f, 0.5f,  -0.5f, 1.0f,   0.647f, 0.0f,   1.0f, // A
      0.5f,  0.5f,  0.5f,  1.0f,   0.647f, 0.0f,   1.0f, // C
      0.5f,  0.5f,  -0.5f, 1.0f,   0.647f, 0.0f,   1.0f, // D

      -0.5f, -0.5f, 0.5f,  0.0f,   1.0f,   0.498f, 1.0f, // A
      -0.5f, -0.5f, -0.5f, 0.0f,   1.0f,   0.498f, 1.0f, // B
      0.5f,  -0.5f, -0.5f, 0.0f,   1.0f,   0.498f, 1.0f, // C
      -0.5f, -0.5f, 0.5f,  0.0f,   1.0f,   0.498f, 1.0f, // A
      0.5f,  -0.5f, -0.5f, 0.0f,   1.0f,   0.498f, 1.0f, // C
      0.5f,  -0.5f, 0.5f,  0.0f,   1.0f,   0.498f, 1.0f, // D

      -0.5f, 0.5f,  0.5f,  0.0f,   0.502f, 0.502f, 1.0f, // A
      -0.5f, -0.5f, 0.5f,  0.0f,   0.502f, 0.502f, 1.0f, // B
      0.5f,  -0.5f, 0.5f,  0.0f,   0.502f, 0.502f, 1.0f, // C
      -0.5f, 0.5f,  0.5f,  0.0f,   0.502f, 0.502f, 1.0f, // A
      0.5f,  -0.5f, 0.5f,  0.0f,   0.502f, 0.502f, 1.0f, // C
      0.5f,  0.5f,  0.5f,  0.0f,   0.502f, 0.502f, 1.0f, // D

      0.5f,  0.5f,  -0.5f, 1.0f,   0.553f, 0.631f, 1.0f, // A
      0.5f,  -0.5f, -0.5f, 1.0f,   0.553f, 0.631f, 1.0f, // B
      -0.5f, -0.5f, -0.5f, 1.0f,   0.553f, 0.631f, 1.0f, // C
      0.5f,  0.5f,  -0.5f, 1.0f,   0.553f, 0.631f, 1.0f, // A
      -0.5f, -0.5f, -0.5f, 1.0f,   0.553f, 0.631f, 1.0f, // C
      -0.5f, 0.5f,  -0.5f, 1.0f,   0.553f, 0.631f, 1.0f, // D

      -0.5f, 0.5f,  -0.5f, 0.678f, 0.847f, 0.902f, 1.0f, // A
      -0.5f, -0.5f, -0.5f, 0.678f, 0.847f, 0.902f, 1.0f, // B
      -0.5f, -0.5f, 0.5f,  0.678f, 0.847f, 0.902f, 1.0f, // C
      -0.5f, 0.5f,  -0.5f, 0.678f, 0.847f, 0.902f, 1.0f, // A
      -0.5f, -0.5f, 0.5f,  0.678f, 0.847f, 0.902f, 1.0f, // C
      -0.5f, 0.5f,  0.5f,  0.678f, 0.847f, 0.902f, 1.0f, // D

      0.5f,  0.5f,  0.5f,  0.8f,   0.76f,  0.89f,  1.0f, // A
      0.5f,  -0.5f, 0.5f,  0.8f,   0.76f,  0.89f,  1.0f, // B
      0.5f,  -0.5f, -0.5f, 0.8f,   0.76f,  0.89f,  1.0f, // C
      0.5f,  0.5f,  0.5f,  0.8f,   0.76f,  0.89f,  1.0f, // A
      0.5f,  -0.5f, -0.5f, 0.8f,   0.76f,  0.89f,  1.0f, // C
      0.5f,  0.5f,  -0.5f, 0.8f,   0.76f,  0.89f,  1.0f, // D
  };
}
