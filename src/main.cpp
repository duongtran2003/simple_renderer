#include "SDL.h"
#include "SDL_events.h"
#include "SDL_pixels.h"
#include "SDL_render.h"
#include "SDL_video.h"
#include <cstddef>
#include <glm/ext/vector_float4.hpp>
#include <glm/glm.hpp>
#include <iostream>

const int WINDOW_W = 640;
const int WINDOW_H = 480;

struct Point {
  int x;
  int y;
  glm::vec4 color;
};

void putPixel(SDL_Texture *texture, Point &point);

int main() {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window =
      SDL_CreateWindow("Rasterizer", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, WINDOW_W, WINDOW_H, 0);

  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

  SDL_Texture *texture = SDL_CreateTexture(
      renderer, SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA8888,
      SDL_TextureAccess::SDL_TEXTUREACCESS_STREAMING, WINDOW_W, WINDOW_H);

  Point p;
  p.x = 25;
  p.y = 25;
  p.color = glm::vec4(133.0f, 50.0f, 168.0f, 255.0f);
  putPixel(texture, p);

  bool isWindowRunning = true;
  SDL_Event e;

  while (isWindowRunning) {
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
        isWindowRunning = false;
      }
      SDL_RenderClear(renderer);
      SDL_RenderCopy(renderer, texture, NULL, NULL);
      SDL_RenderPresent(renderer);
    }
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}

void putPixel(SDL_Texture *texture, Point &point) {
  unsigned char *pixels;
  int pitch;
  SDL_LockTexture(texture, NULL, (void **)&pixels, &pitch);
  int pixelStride = 4;

  int pixelLocation = pitch * point.y + pixelStride * point.x;
  pixels[pixelLocation] = (unsigned char)point.color.x;
  pixels[pixelLocation + 1] = (unsigned char)point.color.x;
  pixels[pixelLocation + 2] = (unsigned char)point.color.y;
  pixels[pixelLocation + 3] = (unsigned char)point.color.z;
  pixels[pixelLocation + 4] = (unsigned char)point.color.w;

  SDL_UnlockTexture(texture);
}
