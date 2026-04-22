#include "hello_world_triangle_pipeline.hpp"
#include "SDL_render.h"
#include "pipeline.hpp"
#include <cstddef>
#include <glm/ext/vector_float4.hpp>
#include <glm/fwd.hpp>
#include <vector>

namespace SimpleRenderer {

HelloWorldTrianglePipeline::HelloWorldTrianglePipeline(SDL_Renderer *renderer)
    : Pipeline<HelloWorldTriangleVertex, HelloWorldTriangleVertexShaderOutput>(
          renderer) {}

std::vector<HelloWorldTriangleVertex>
HelloWorldTrianglePipeline::assembleInput(const std::vector<float> &rawInput) {
  std::vector<HelloWorldTriangleVertex> vertices;

  size_t stride = 7;
  size_t positionOffset = 0;
  size_t colorOffset = 3;

  if (rawInput.size() <= stride) {
    return vertices;
  }

  for (size_t i = 0; i < rawInput.size(); i += stride) {
    glm::vec3 position = glm::vec3(rawInput[i + positionOffset],
                                   rawInput[i + positionOffset + 1],
                                   rawInput[i + positionOffset + 2]);

    glm::vec4 color =
        glm::vec4(rawInput[i + colorOffset], rawInput[i + colorOffset + 1],
                  rawInput[i + colorOffset + 2], rawInput[i + colorOffset + 3]);

    HelloWorldTriangleVertex vertex = {.position = position, .color = color};

    vertices.push_back(vertex);
  }

  return vertices;
}

std::vector<HelloWorldTrianglePipeline::VertexShaderOut>
HelloWorldTrianglePipeline::vertexShader(
    const std::vector<HelloWorldTriangleVertex> &vertices) {
  std::vector<HelloWorldTrianglePipeline::VertexShaderOut> out;

  glm::mat4 projection = std::get<glm::mat4>(getUniform("u_projectionMatrix"));
  glm::mat4 view = std::get<glm::mat4>(getUniform("u_viewMatrix"));
  glm::mat4 model = std::get<glm::mat4>(getUniform("u_modelMatrix"));

  for (const auto &v : vertices) {
    HelloWorldTrianglePipeline::VertexShaderOut vOut = {
        .coords = projection * view * model * glm::vec4(v.position, 1.0f),
        .vsOut = {.color = v.color}};

    out.push_back(vOut);
  }

  return out;
}

glm::vec4 HelloWorldTrianglePipeline::fragmentShader(
    const HelloWorldTrianglePipeline::RasterizerOutput &fragment) {

  glm::vec4 color = fragment.vsOut1.color * fragment.barycentric.x +
                    fragment.vsOut2.color * fragment.barycentric.y +
                    fragment.vsOut3.color * fragment.barycentric.z;
  return color;
}

} // namespace SimpleRenderer
