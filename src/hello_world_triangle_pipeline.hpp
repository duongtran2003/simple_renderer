#pragma once

#include "SDL_render.h"
#include "pipeline.hpp"
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include <vector>

namespace SimpleRenderer {

struct HelloWorldTriangleVertex {
  glm::vec3 position;
  glm::vec3 normal;
};

struct HelloWorldTriangleVertexShaderOutput {
  glm::vec3 worldSpaceCoords;
  glm::vec3 normal;
};

class HelloWorldTrianglePipeline
    : public Pipeline<HelloWorldTriangleVertex,
                      HelloWorldTriangleVertexShaderOutput> {
public:
  HelloWorldTrianglePipeline(SDL_Renderer *renderer);

protected:
  std::vector<HelloWorldTriangleVertex>
  assembleInput(const std::vector<float> &rawInput) override;

  virtual std::vector<VertexShaderOut>
  vertexShader(const std::vector<HelloWorldTriangleVertex> &vertices) override;

  virtual glm::vec4 fragmentShader(const RasterizerOutput &fragment) override;
};
} // namespace SimpleRenderer
