#pragma once

#include <cstdint>

namespace SimpleRenderer {
class App {
public:
  uint64_t lastFrameTicks;
  double deltaTime;

  App();
  ~App();

  void init();
  void calculateDeltaTime();
};
} // namespace SimpleRenderer
