#pragma once

#include "SDL_pixels.h"
#include "SDL_render.h"
#include <algorithm>
#include <cfloat>
#include <cstddef>
#include <glm/common.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <unistd.h>
#include <unordered_map>
#include <variant>
#include <vector>

namespace SimpleRenderer {
template <typename VertexType, typename VertexShaderOutput> class Pipeline {
public:
  using UniformValue = std::variant<float, int, bool, glm::vec2, glm::vec3,
                                    glm::vec4, glm::mat3, glm::mat4>;
  enum DepthFunc {
    Never,
    Less,
    Equal,
    LessAndEqual,
    Greater,
    NotEqual,
    GreaterAndEqual,
    Always
  };

  struct VertexShaderOut {
    glm::vec4 coords;
    VertexShaderOutput vsOut;
  };

  struct Primitive {
    VertexShaderOut v1;
    VertexShaderOut v2;
    VertexShaderOut v3;
  };

  struct RasterizerOutput {
    glm::vec2 screenCoords;
    float interpolatedZ;
    float interpolatedInversedW;
    VertexShaderOutput vsOut1;
    VertexShaderOutput vsOut2;
    VertexShaderOutput vsOut3;
    glm::vec3 barycentric;
  };

  inline static constexpr int BUFFER_W = 640;
  inline static constexpr int BUFFER_H = 480;
  inline static constexpr glm::vec4 CLEAR_COLOR =
      glm::vec4(0.0f, 0.0f, 0.0f, 255.0f);

  Pipeline(SDL_Renderer *renderer);
  virtual ~Pipeline();
  Pipeline &setDepthTest(bool isEnable);
  Pipeline &setDepthMask(bool isEnable);
  Pipeline &setEarlyZ(bool isEnable);
  Pipeline &setDepthTestFunc(DepthFunc func);
  Pipeline &setBufferSize(int width, int height);
  Pipeline &setClearColor(glm::vec4 color);
  Pipeline &clearDepthBuffer();
  Pipeline &clearColorBuffer();

  Pipeline &setUniform(std::string name, UniformValue value);

  void render(const std::vector<float> &rawData);

  SDL_Texture *getDrawnFrame();

protected:
  bool isDepthTestEnable;
  bool depthMask;
  bool isEarlyZEnable;

  DepthFunc depthFunc;

  std::vector<float> depthBuffer;
  std::vector<glm::vec4> colorBuffer;
  glm::vec2 bufferSize;
  SDL_Texture *drawnFrame;

  glm::vec4 clearColor;

  std::unordered_map<std::string, UniformValue> uniforms;

  UniformValue getUniform(std::string name);

  virtual std::vector<VertexType>
  assembleInput(const std::vector<float> &rawInput) = 0;

  virtual std::vector<VertexShaderOut>
  vertexShader(const std::vector<VertexType> &vertices) = 0;

  std::vector<Primitive>
  assemblePrimitives(const std::vector<VertexShaderOut> &vertices);

  virtual glm::vec4 fragmentShader(const RasterizerOutput &fragment) = 0;

  bool depthTest(float zValue, glm::vec2 location);
  std::vector<RasterizerOutput> rasterize(const Primitive &primitive);
  void writeColor(glm::vec4 color, glm::vec2 location);
};

template <typename VertexType, typename VertexShaderOutput>
Pipeline<VertexType, VertexShaderOutput>::Pipeline(SDL_Renderer *renderer) {
  isDepthTestEnable = false;
  isEarlyZEnable = false;
  depthMask = false;
  setBufferSize(BUFFER_W, BUFFER_H);
  setClearColor(CLEAR_COLOR);
  drawnFrame =
      SDL_CreateTexture(renderer, SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA32,
                        SDL_TextureAccess::SDL_TEXTUREACCESS_STREAMING,
                        bufferSize.x, bufferSize.y);
}

template <typename VertexType, typename VertexShaderOutput>
Pipeline<VertexType, VertexShaderOutput>::~Pipeline() {
  SDL_DestroyTexture(drawnFrame);
}

template <typename VertexType, typename VertexShaderOutput>
Pipeline<VertexType, VertexShaderOutput> &
Pipeline<VertexType, VertexShaderOutput>::setDepthTest(bool isEnable) {
  isDepthTestEnable = isEnable;
  return *this;
}

template <typename VertexType, typename VertexShaderOutput>
Pipeline<VertexType, VertexShaderOutput> &
Pipeline<VertexType, VertexShaderOutput>::setDepthMask(bool isEnable) {
  depthMask = isEnable;
  return *this;
}

template <typename VertexType, typename VertexShaderOutput>
Pipeline<VertexType, VertexShaderOutput> &
Pipeline<VertexType, VertexShaderOutput>::setEarlyZ(bool isEnable) {
  isEarlyZEnable = isEnable;
  return *this;
}

template <typename VertexType, typename VertexShaderOutput>
Pipeline<VertexType, VertexShaderOutput> &
Pipeline<VertexType, VertexShaderOutput>::setDepthTestFunc(DepthFunc func) {
  depthFunc = func;
  return *this;
}

template <typename VertexType, typename VertexShaderOutput>
Pipeline<VertexType, VertexShaderOutput> &
Pipeline<VertexType, VertexShaderOutput>::setBufferSize(int width, int height) {
  bufferSize = glm::vec2(width, height);
  colorBuffer.assign(width * height, clearColor);
  depthBuffer.assign(width * height, FLT_MAX);

  return *this;
}

template <typename VertexType, typename VertexShaderOutput>
Pipeline<VertexType, VertexShaderOutput> &
Pipeline<VertexType, VertexShaderOutput>::setClearColor(glm::vec4 color) {
  clearColor = color;
  return *this;
}

template <typename VertexType, typename VertexShaderOutput>
Pipeline<VertexType, VertexShaderOutput> &
Pipeline<VertexType, VertexShaderOutput>::clearDepthBuffer() {
  std::fill(depthBuffer.begin(), depthBuffer.end(), FLT_MAX);
  return *this;
}

template <typename VertexType, typename VertexShaderOutput>
Pipeline<VertexType, VertexShaderOutput> &
Pipeline<VertexType, VertexShaderOutput>::clearColorBuffer() {
  std::fill(colorBuffer.begin(), colorBuffer.end(), clearColor);
  return *this;
}

template <typename VertexType, typename VertexShaderOutput>
Pipeline<VertexType, VertexShaderOutput> &
Pipeline<VertexType, VertexShaderOutput>::setUniform(std::string name,
                                                     UniformValue value) {
  uniforms[name] = value;
  return *this;
}

template <typename VertexType, typename VertexShaderOutput>
typename Pipeline<VertexType, VertexShaderOutput>::UniformValue
Pipeline<VertexType, VertexShaderOutput>::getUniform(std::string name) {
  auto it = uniforms.find(name);
  if (it != uniforms.end()) {
    return it->second;
  }

  return UniformValue{};
}

template <typename VertexType, typename VertexShaderOutput>
std::vector<typename Pipeline<VertexType, VertexShaderOutput>::Primitive>
Pipeline<VertexType, VertexShaderOutput>::assemblePrimitives(
    const std::vector<VertexShaderOut> &vertices) {
  std::vector<Primitive> primitives;

  if (vertices.size() < 3) {
    return primitives;
  }

  for (int i = 0; i < vertices.size() - 2; i += 3) {
    Primitive primitive = {
        .v1 = vertices[i],
        .v2 = vertices[i + 1],
        .v3 = vertices[i + 2],
    };

    primitives.push_back(primitive);
  }

  return primitives;
}

template <typename VertexType, typename VertexShaderOutput>
bool Pipeline<VertexType, VertexShaderOutput>::depthTest(float zValue,
                                                         glm::vec2 location) {
  int depthIndex = location.y * bufferSize.x + location.x;
  float depthBufferValue = depthBuffer[depthIndex];
  bool isPassed = false;

  switch (depthFunc) {
  case Never:
    isPassed = false;
    break;
  case Less:
    isPassed = zValue < depthBufferValue;
    break;
  case Equal:
    isPassed = zValue == depthBufferValue;
    break;
  case LessAndEqual:
    isPassed = zValue <= depthBufferValue;
    break;
  case Greater:
    isPassed = zValue > depthBufferValue;
    break;
  case NotEqual:
    isPassed = zValue != depthBufferValue;
    break;
  case GreaterAndEqual:
    isPassed = zValue >= depthBufferValue;
    break;
  case Always:
    isPassed = true;
    break;
  default:
    isPassed = false;
  }

  if (depthMask && isPassed) {
    depthBuffer[depthIndex] = zValue;
  }

  return isPassed;
}

template <typename VertexType, typename VertexShaderOutput>
std::vector<typename Pipeline<VertexType, VertexShaderOutput>::RasterizerOutput>
Pipeline<VertexType, VertexShaderOutput>::rasterize(
    const Pipeline<VertexType, VertexShaderOutput>::Primitive &primitive) {

  Primitive ndc = primitive;
  ndc.v1.coords /= ndc.v1.coords.w;
  ndc.v2.coords /= ndc.v2.coords.w;
  ndc.v3.coords /= ndc.v3.coords.w;

  Primitive ss = ndc;
  ss.v1.coords.x = ((ss.v1.coords.x + 1.0f) / 2.0f) * bufferSize.x;
  ss.v1.coords.y = ((ss.v1.coords.y * -1.0f + 1.0f) / 2.0f) * bufferSize.y;

  ss.v2.coords.x = ((ss.v2.coords.x + 1.0f) / 2.0f) * bufferSize.x;
  ss.v2.coords.y = ((ss.v2.coords.y * -1.0f + 1.0f) / 2.0f) * bufferSize.y;

  ss.v3.coords.x = ((ss.v3.coords.x + 1.0f) / 2.0f) * bufferSize.x;
  ss.v3.coords.y = ((ss.v3.coords.y * -1.0f + 1.0f) / 2.0f) * bufferSize.y;

  float upper =
      glm::min(ss.v1.coords.y, glm::min(ss.v2.coords.y, ss.v3.coords.y));
  float lower =
      glm::max(ss.v1.coords.y, glm::max(ss.v2.coords.y, ss.v3.coords.y));
  float left =
      glm::min(ss.v1.coords.x, glm::min(ss.v2.coords.x, ss.v3.coords.x));
  float right =
      glm::max(ss.v1.coords.x, glm::max(ss.v2.coords.x, ss.v3.coords.x));

  float invW1 = 1.0f / primitive.v1.coords.w;
  float invW2 = 1.0f / primitive.v2.coords.w;
  float invW3 = 1.0f / primitive.v3.coords.w;

  std::vector<RasterizerOutput> fragments;

  for (int pY = (int)upper; pY <= (int)lower; pY++) {
    for (int pX = (int)left; pX <= (int)right; pX++) {
      // Barycentric calculation
      float area =
          (ss.v3.coords.x - ss.v1.coords.x) *
              (ss.v2.coords.y - ss.v1.coords.y) -
          (ss.v3.coords.y - ss.v1.coords.y) * (ss.v2.coords.x - ss.v1.coords.x);

      if (glm::abs(area) < 0.0001f) {
        continue;
      }

      float u = ((pX - ss.v2.coords.x) * (ss.v3.coords.y - ss.v2.coords.y) -
                 (pY - ss.v2.coords.y) * (ss.v3.coords.x - ss.v2.coords.x)) /
                area;

      float v = ((pX - ss.v3.coords.x) * (ss.v1.coords.y - ss.v3.coords.y) -
                 (pY - ss.v3.coords.y) * (ss.v1.coords.x - ss.v3.coords.x)) /
                area;

      float w = 1.0f - u - v;

      float precisionTolerance = -0.0001f;
      bool isInsidePrimitive = u > precisionTolerance &&
                               v > precisionTolerance && w > precisionTolerance;

      if (!isInsidePrimitive) {
        continue;
      }

      float interpolatedInversedW = (invW1 * u) + (invW2 * v) + (invW3 * w);

      float z1 = primitive.v1.coords.z * invW1;
      float z2 = primitive.v2.coords.z * invW2;
      float z3 = primitive.v3.coords.z * invW3;

      float interpolatedZ = (z1 * u + z2 * v + z3 * w) / interpolatedInversedW;

      RasterizerOutput output = {
          .screenCoords = glm::vec2(pX, pY),
          .interpolatedZ = interpolatedZ,
          .interpolatedInversedW = interpolatedInversedW,
          .vsOut1 = primitive.v1.vsOut,
          .vsOut2 = primitive.v2.vsOut,
          .vsOut3 = primitive.v3.vsOut,
          .barycentric = glm::vec3(u, v, w),
      };

      fragments.push_back(output);
    }
  }

  return fragments;
}

template <typename VertexType, typename VertexShaderOutput>
void Pipeline<VertexType, VertexShaderOutput>::render(
    const std::vector<float> &rawInput) {
  std::vector<VertexType> vertices = assembleInput(rawInput);
  std::vector<VertexShaderOut> vertexOut = vertexShader(vertices);
  std::vector<Primitive> primitives = assemblePrimitives(vertexOut);

  for (const Primitive &primitive : primitives) {
    std::vector<RasterizerOutput> fragments = rasterize(primitive);
    for (const RasterizerOutput &fragment : fragments) {
      glm::vec4 fragColor;
      if (isDepthTestEnable) {
        if (isEarlyZEnable) {
          bool isPassedDepthTest =
              depthTest(fragment.interpolatedZ, fragment.screenCoords);

          if (!isPassedDepthTest) {
            continue;
          }

          fragColor = fragmentShader(fragment);
        } else {
          fragColor = fragmentShader(fragment);
          bool isPassedDepthTest =
              depthTest(fragment.interpolatedZ, fragment.screenCoords);

          if (!isPassedDepthTest) {
            continue;
          }
        }
      } else {
        fragColor = fragmentShader(fragment);
      }

      writeColor(fragColor, fragment.screenCoords);
    }
  }
}

template <typename VertexType, typename VertexShaderOutput>
void Pipeline<VertexType, VertexShaderOutput>::writeColor(glm::vec4 color,
                                                          glm::vec2 location) {
  int colorIndex = location.y * bufferSize.x + location.x;
  colorBuffer[colorIndex] = color;
}

template <typename VertexType, typename VertexShaderOutput>
SDL_Texture *Pipeline<VertexType, VertexShaderOutput>::getDrawnFrame() {

  unsigned char *pixels;
  int pitch;
  SDL_LockTexture(drawnFrame, NULL, (void **)&pixels, &pitch);
  int pixelStride = 4;

  for (int i = 0; i < colorBuffer.size(); i++) {
    int x = i % (int)bufferSize.x;
    int y = i / (int)bufferSize.x;

    glm::vec4 color = colorBuffer[i];

    int pixelLocation = pitch * y + pixelStride * x;
    pixels[pixelLocation] =
        (unsigned char)(glm::clamp(color.r, 0.0f, 1.0f) * 255.0f);
    pixels[pixelLocation + 1] =
        (unsigned char)(glm::clamp(color.g, 0.0f, 1.0f) * 255.0f);
    pixels[pixelLocation + 2] =
        (unsigned char)(glm::clamp(color.b, 0.0f, 1.0f) * 255.0f);
    pixels[pixelLocation + 3] =
        (unsigned char)(glm::clamp(color.a, 0.0f, 1.0f) * 255.0f);
  }

  SDL_UnlockTexture(drawnFrame);
  return drawnFrame;
}
} // namespace SimpleRenderer
