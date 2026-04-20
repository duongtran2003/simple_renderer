#include "app.hpp"
#include "SDL_events.h"
#include "SDL_keycode.h"
#include "input.hpp"
#include "window.hpp"
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

struct Vertex {
  glm::vec3 coords;
  glm::vec4 color;
};

struct Primitive {
  Vertex v1;
  Vertex v2;
  Vertex v3;
};

glm::vec3 queryBarycentricCoords(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3,
                                 glm::vec3 queryPoint);
bool isInsidePrimitive(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3,
                       glm::vec3 queryPoint);
glm::vec4 getBoundingBox(Primitive primitive);

App::App() {}
void App::init() {
  Window *window = new Window("My simple renderer", 640, 480);
  Input &input = Input::getInstance();

  std::vector<Vertex> vertices;
  vertices.push_back(
      {glm::vec3(320.0f, 100.0f, 0.0f), glm::vec4(255.0f, 0.0f, 0.0f, 255.0f)});
  vertices.push_back(
      {glm::vec3(120.0f, 380.0f, 0.0f), glm::vec4(0.0f, 255.0f, 0.0f, 255.0f)});
  vertices.push_back(
      {glm::vec3(520.0f, 380.0f, 0.0f), glm::vec4(0.0f, 0.0f, 255.0f, 255.0f)});

  Primitive primitive = {
      .v1 = vertices[0],
      .v2 = vertices[1],
      .v3 = vertices[2],
  };

  glm::vec4 boundingBox = getBoundingBox(primitive);
  std::cout << boundingBox.x << " " << boundingBox.y << " " << boundingBox.z
            << " " << boundingBox.w << "\n";

  SDL_Event e;

  while (window->isWindowRunning()) {
    while (SDL_PollEvent(&e) != 0) {
      input.processInput(e);
      if (input.getShouldQuit() || input.isKeyPressed(SDLK_ESCAPE)) {
        window->closeWindow();
        return;
      }
    }

    for (int pY = (int)boundingBox.x; pY <= (int)boundingBox.y; pY++) {
      for (int pX = (int)boundingBox.z; pX <= (int)boundingBox.w; pX++) {
        bool isInside =
            isInsidePrimitive(primitive.v1.coords, primitive.v2.coords,
                              primitive.v3.coords, glm::vec3(pX, pY, 0.0f));

        if (isInside) {
          glm::vec3 barycentric = queryBarycentricCoords(
              primitive.v1.coords, primitive.v2.coords, primitive.v3.coords,
              glm::vec3(pX, pY, 0.0f));
          glm::vec3 interpolatedColor = barycentric.x * primitive.v1.color +
                                        barycentric.y * primitive.v2.color +
                                        barycentric.z * primitive.v3.color;
          Point p = {
              .x = pX, .y = pY, .color = glm::vec4(interpolatedColor, 255.0f)};

          window->putPixel(p);
        }
      }
    }

    window->swapFramebuffer();
  }

  return;
}

float triangleAreaCalc(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) {
  // Edge function
  return (v3.x - v1.x) * (v2.y - v1.y) - (v3.y - v1.y) * (v2.x - v1.x);
}

glm::vec3 queryBarycentricCoords(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3,
                                 glm::vec3 queryPoint) {
  float fullArea = triangleAreaCalc(v1, v2, v3);

  float u = triangleAreaCalc(v2, v3, queryPoint) / fullArea;
  float v = triangleAreaCalc(v3, v1, queryPoint) / fullArea;
  float w = 1.0f - u - v;

  return glm::vec3(u, v, w);
}

bool isInsidePrimitive(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3,
                       glm::vec3 queryPoint) {
  glm::vec3 bary = queryBarycentricCoords(v1, v2, v3, queryPoint);

  float precisionTolerance = -0.0001f;
  if (bary.x < precisionTolerance || bary.y < precisionTolerance ||
      bary.z < precisionTolerance) {
    return false;
  }

  return true;
}

glm::vec4 getBoundingBox(Primitive primitive) {
  // Screen space coord system starts from top-left;

  float upper =
      glm::min(primitive.v1.coords.y,
               glm::min(primitive.v2.coords.y, primitive.v3.coords.y));
  float lower =
      glm::max(primitive.v1.coords.y,
               glm::max(primitive.v2.coords.y, primitive.v3.coords.y));
  float left = glm::min(primitive.v1.coords.x,
                        glm::min(primitive.v2.coords.x, primitive.v3.coords.x));
  float right =
      glm::max(primitive.v1.coords.x,
               glm::max(primitive.v2.coords.x, primitive.v3.coords.x));

  return glm::vec4(upper, lower, left, right);
}
} // namespace SimpleRenderer
