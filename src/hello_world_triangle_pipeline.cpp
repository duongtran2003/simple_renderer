#include "hello_world_triangle_pipeline.hpp"
#include "SDL_render.h"
#include "pipeline.hpp"
#include <cstddef>
#include <glm/common.hpp>
#include <glm/exponential.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <vector>

namespace SimpleRenderer {

HelloWorldTrianglePipeline::HelloWorldTrianglePipeline(SDL_Renderer *renderer)
    : Pipeline<HelloWorldTriangleVertex, HelloWorldTriangleVertexShaderOutput>(
          renderer) {}

std::vector<HelloWorldTriangleVertex>
HelloWorldTrianglePipeline::assembleInput(const std::vector<float> &rawInput) {
  std::vector<HelloWorldTriangleVertex> vertices;

  size_t stride = 6;
  size_t positionOffset = 0;
  size_t normalOffset = 3;

  if (rawInput.size() <= stride) {
    return vertices;
  }

  for (size_t i = 0; i < rawInput.size(); i += stride) {
    glm::vec3 position = glm::vec3(rawInput[i + positionOffset],
                                   rawInput[i + positionOffset + 1],
                                   rawInput[i + positionOffset + 2]);

    glm::vec3 normal =
        glm::vec3(rawInput[i + normalOffset], rawInput[i + normalOffset + 1],
                  rawInput[i + normalOffset + 2]);

    HelloWorldTriangleVertex vertex = {.position = position, .normal = normal};

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
    HelloWorldTriangleVertexShaderOutput vsOut = {
        .worldSpaceCoords = glm::vec3(model * glm::vec4(v.position, 1.0f)),
        .normal = glm::vec3(model * glm::vec4(v.normal, 1.0f)),
    };

    HelloWorldTrianglePipeline::VertexShaderOut vOut = {
        .coords = projection * view * model * glm::vec4(v.position, 1.0f),
        .vsOut = vsOut};

    out.push_back(vOut);
  }

  return out;
}

glm::vec4 HelloWorldTrianglePipeline::fragmentShader(
    const HelloWorldTrianglePipeline::RasterizerOutput &fragment) {

  glm::vec4 color =
      glm::vec4(std::get<glm::vec3>(getUniform("u_cubeColor")), 1.0f);

  float ambientStrength = 0.2f;
  glm::vec4 ambientColor = color * ambientStrength;

  glm::vec3 interpolatedNormal =
      fragment.vsOut1.normal * fragment.barycentric.x +
      fragment.vsOut2.normal * fragment.barycentric.y +
      fragment.vsOut3.normal * fragment.barycentric.z;
  interpolatedNormal = glm::normalize(interpolatedNormal);
  glm::vec3 interpolatedWorldSpaceCoords =
      fragment.vsOut1.worldSpaceCoords * fragment.barycentric.x +
      fragment.vsOut2.worldSpaceCoords * fragment.barycentric.y +
      fragment.vsOut3.worldSpaceCoords * fragment.barycentric.z;

  glm::vec3 lightPosition = std::get<glm::vec3>(getUniform("u_lightPosition"));
  glm::vec3 lightDirection = lightPosition - interpolatedWorldSpaceCoords;
  lightDirection = glm::normalize(lightDirection);

  float diffuseStrength =
      glm::clamp(glm::dot(lightDirection, interpolatedNormal), 0.0f, 1.0f);

  glm::vec4 diffuseColor = color * diffuseStrength;

  float specularStrength = 0.0f;
  if (diffuseStrength > 0.0f) {
    glm::vec3 cameraPosition =
        std::get<glm::vec3>(getUniform("u_cameraPosition"));

    glm::vec3 viewDirection = cameraPosition - interpolatedWorldSpaceCoords;
    glm::vec3 halfAngleVector =
        glm::normalize(viewDirection) + glm::normalize(lightDirection);

    halfAngleVector = glm::normalize(halfAngleVector);

    float shininess = std::get<float>(getUniform("u_shininess"));

    specularStrength = glm::pow(
        glm::clamp(glm::dot(halfAngleVector, interpolatedNormal), 0.0f, 1.0f),
        shininess);
  }

  glm::vec4 specularColor = specularStrength * color;

  glm::vec4 fragmentColor =
      glm::vec4(glm::vec3(diffuseColor + ambientColor + specularColor), 1.0f);

  return fragmentColor;
}

} // namespace SimpleRenderer
