#include "window.hpp"
#include "SDL.h"
#include "SDL_pixels.h"
#include "SDL_render.h"
#include "SDL_video.h"
#include <cstddef>
#include <glm/ext/vector_float4.hpp>
#include <string>

namespace SimpleRenderer {
Window::Window(std::string title, int windowW, int windowH) {
  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, windowW, windowH, 0);

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

  isRunning = true;
}

void Window::swapFramebuffer(SDL_Texture *frame) const {
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, frame, NULL, NULL);
  SDL_RenderPresent(renderer);
}

void Window::closeWindow() { isRunning = false; }

bool Window::isWindowRunning() const { return isRunning; }

SDL_Renderer *Window::getRenderer() const { return renderer; }

Window::~Window() {
  if (renderer) {
    SDL_DestroyRenderer(renderer);
  }
  if (window) {
    SDL_DestroyWindow(window);
  }

  SDL_Quit();
}
} // namespace SimpleRenderer
