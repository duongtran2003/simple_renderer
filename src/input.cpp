#include "input.hpp"
#include "SDL_events.h"
#include "SDL_keycode.h"
#include "SDL_stdinc.h"
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <unordered_map>

namespace SimpleRenderer {
Input::Input() { shouldQuit = false; }

bool Input::getShouldQuit() const { return shouldQuit; }

Input &Input::getInstance() {
  static Input instance;
  return instance;
}

const std::unordered_map<SDL_Keycode, bool> &Input::getKeysPressed() const {
  return keysPressed;
}

const std::unordered_map<Uint16, bool> &Input::getModifierKeysPressed() const {
  return modifierKeysPressed;
}

bool Input::isKeyPressed(SDL_Keycode key) { return keysPressed[key]; }

bool Input::isModifierKeyPressed(Uint16 key) {
  return modifierKeysPressed[key];
}

glm::vec2 Input::getMousePos() const { return glm::vec2(mouseX, mouseY); }

glm::vec2 Input::getLastMousePos() const {
  return glm::vec2(lastMouseX, lastMouseY);
}

void Input::processInput(SDL_Event &e) {
  lastMouseX = mouseX;
  lastMouseY = mouseY;
  switch (e.type) {
  case SDL_QUIT:
    shouldQuit = true;
    return;
  case SDL_KEYDOWN:
    keysPressed[e.key.keysym.sym] = true;
    break;
  case SDL_KEYUP:
    keysPressed[e.key.keysym.sym] = false;
    break;
  case SDL_MOUSEMOTION:
    mouseX = e.motion.x;
    mouseY = e.motion.y;
    break;
  default:
    break;
  }
}
} // namespace SimpleRenderer
