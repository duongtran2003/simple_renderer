#include "SDL.h"
#include "SDL_pixels.h"
#include "SDL_render.h"
#include "SDL_timer.h"
#include "SDL_video.h"
#include <glm/glm.hpp>
#include <iostream>

const int WINDOW_W = 640;
const int WINDOW_H = 480;

int main() {
  std::cout << "Hello world";

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window =
      SDL_CreateWindow("Rasterizer", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, WINDOW_W, WINDOW_H, 0);

  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);

  SDL_Delay(3000);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
