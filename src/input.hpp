#pragma once

#include "SDL_events.h"
#include "SDL_keycode.h"
#include "SDL_stdinc.h"
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <unordered_map>

namespace SimpleRenderer {
class Input {
public:
  static Input &getInstance();
  Input(const Input &) = delete;
  Input &operator=(const Input &) = delete;

  bool getShouldQuit() const;

  void processInput(SDL_Event &e);

  const std::unordered_map<SDL_Keycode, bool> &getKeysPressed() const;
  const std::unordered_map<Uint16, bool> &getModifierKeysPressed() const;

  bool isKeyPressed(SDL_Keycode key);
  bool isModifierKeyPressed(Uint16 key);

  glm::vec2 getMouseRel() const;
  void clearMouseRel();

private:
  Input();

  std::unordered_map<SDL_Keycode, bool> keysPressed;
  std::unordered_map<Uint16, bool> modifierKeysPressed;

  bool shouldQuit;

  glm::vec2 mouseRelative;
};
} // namespace SimpleRenderer
