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

  framebuffer = SDL_CreateTexture(
      renderer, SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA8888,
      SDL_TextureAccess::SDL_TEXTUREACCESS_STREAMING, windowW, windowH);

  isRunning = true;
}

void Window::swapFramebuffer() {
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, framebuffer, NULL, NULL);
  SDL_RenderPresent(renderer);
}

void Window::closeWindow() { isRunning = false; }

bool Window::isWindowRunning() { return isRunning; }

Window::~Window() {
  if (framebuffer) {
    SDL_DestroyTexture(framebuffer);
  }
  if (renderer) {
    SDL_DestroyRenderer(renderer);
  }
  if (window) {
    SDL_DestroyWindow(window);
  }

  SDL_Quit();
}

void Window::putPixel(Point &point) {
  unsigned char *pixels;
  int pitch;
  SDL_LockTexture(framebuffer, NULL, (void **)&pixels, &pitch);
  int pixelStride = 4;

  int pixelLocation = pitch * point.y + pixelStride * point.x;
  pixels[pixelLocation] = (unsigned char)point.color.x;
  pixels[pixelLocation + 1] = (unsigned char)point.color.x;
  pixels[pixelLocation + 2] = (unsigned char)point.color.y;
  pixels[pixelLocation + 3] = (unsigned char)point.color.z;
  pixels[pixelLocation + 4] = (unsigned char)point.color.w;

  SDL_UnlockTexture(framebuffer);
}
} // namespace SimpleRenderer
