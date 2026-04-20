#include "app.hpp"
#include <glm/ext/vector_float4.hpp>
#include <glm/glm.hpp>

int main() {
  SimpleRenderer::App *app = new SimpleRenderer::App();
  app->init();
  return 0;
}
