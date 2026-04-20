#include "input.hpp"
#include "SDL_events.h"
#include "SDL_keycode.h"
#include "SDL_stdinc.h"
#include <unordered_map>

namespace SimpleRenderer {
Input::Input() { shouldQuit = false; }

bool Input::getShouldQuit() { return shouldQuit; }

Input &Input::getInstance() {
  static Input instance;
  return instance;
}

std::unordered_map<SDL_Keycode, bool> &Input::getKeysPressed() {
  return keysPressed;
}

std::unordered_map<Uint16, bool> &Input::getModifierKeysPressed() {
  return modifierKeysPressed;
}

bool Input::isKeyPressed(SDL_Keycode key) { return keysPressed[key]; }

bool Input::isModifierKeyPressed(Uint16 key) {
  return modifierKeysPressed[key];
}

void Input::processInput(SDL_Event &e) {
  if (e.type == SDL_QUIT) {
    shouldQuit = true;
    return;
  }

  if (e.type == SDL_KEYDOWN) {
    keysPressed[e.key.keysym.sym] = true;
  } else if (e.type == SDL_KEYUP) {
    keysPressed[e.key.keysym.sym] = false;
  }
}
} // namespace SimpleRenderer
