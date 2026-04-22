#include "app.hpp"
#include "SDL_events.h"
#include "SDL_keycode.h"
#include "SDL_timer.h"
#include "camera.hpp"
#include "hello_world_triangle_pipeline.hpp"
#include "input.hpp"
#include "pipeline.hpp"
#include "window.hpp"
#include <chrono>
#include <cstdint>
#include <ctime>
#include <glm/common.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <vector>

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
  const int WINDOW_W = 640;
  const int WINDOW_H = 480;

  Window *window = new Window("My simple renderer", WINDOW_W, WINDOW_H);
  Input &input = Input::getInstance();

  std::vector<float> rawData = {
      0.0f, 0.5f, 0.0f, 1.0f, 0.0f,  0.0f, 1.0f, -0.5f, -0.5f, 0.0f, 0.0f,
      1.0f, 0.0f, 1.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  1.0f,  1.0f,
  };

  SDL_Event e;

  HelloWorldTrianglePipeline *helloWorldTrianglePipeline =
      new HelloWorldTrianglePipeline(window->getRenderer());

  helloWorldTrianglePipeline->setDepthTest(true)
      .setDepthMask(true)
      .setEarlyZ(false)
      .setDepthTestFunc(HelloWorldTrianglePipeline::Pipeline::Less)
      .setBufferSize(WINDOW_W, WINDOW_H)
      .setClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

  Camera *camera = new Camera();
  camera->setPosition(glm::vec3(0.0f, 0.0f, 3.0f))
      .setDirection(glm::vec3(0.0f, 0.0f, -1.0f))
      .setWorldUp(glm::vec3(0.0f, 1.0f, 0.0f))
      .setYaw(-90.0f)
      .setPitch(0.0f)
      .setFov(90.0f)
      .setWidth(WINDOW_W)
      .setHeight(WINDOW_H)
      .setNear(1.0f)
      .setFar(100.0f);

  while (window->isWindowRunning()) {
    calculateDeltaTime();
    while (SDL_PollEvent(&e) != 0) {
      input.processInput(e);
      if (input.getShouldQuit() || input.isKeyPressed(SDLK_ESCAPE)) {
        window->closeWindow();
        return;
      }
    }

    helloWorldTrianglePipeline->clearColorBuffer();
    helloWorldTrianglePipeline->clearDepthBuffer();
    helloWorldTrianglePipeline->render(rawData);
    window->swapFramebuffer(helloWorldTrianglePipeline->getDrawnFrame());

    std::cout << "Frametime: " << deltaTime << "ms\n";
  }

  return;
}
} // namespace SimpleRenderer
