#pragma once

#include "SDL_render.h"
#include "SDL_video.h"
#include <glm/ext/vector_float4.hpp>
#include <string>
namespace SimpleRenderer {
struct Point {
  int x;
  int y;
  glm::vec4 color;
};

class Window {
private:
  SDL_Window *window;
  SDL_Renderer *renderer;
  bool isRunning;

  ~Window();

public:
  inline static constexpr int WIDTH = 800;
  inline static constexpr int HEIGHT = 600;
  inline static std::string TITLE = "Simple Renderer";

  Window(std::string title = TITLE, int windowW = WIDTH, int windowH = HEIGHT);
  void swapFramebuffer(SDL_Texture *frame) const;
  void closeWindow();
  bool isWindowRunning() const;
  SDL_Renderer *getRenderer() const;
};
} // namespace SimpleRenderer
