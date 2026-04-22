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

glm::vec2 Input::getMouseRel() const { return mouseRelative; }

void Input::processInput(SDL_Event &e) {
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
    mouseRelative.x += e.motion.xrel;
    mouseRelative.y += e.motion.yrel;
    break;
  default:
    break;
  }
}

void Input::clearMouseRel() { mouseRelative = glm::vec2(0.0f, 0.0f); }
} // namespace SimpleRenderer
