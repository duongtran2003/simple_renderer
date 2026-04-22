# Simple Software Rendering
## What is this?
This project is a Work-In-Progress (WIP) playground designed to demonstrate and practice fundamental concepts in Computer Graphics.
This project intentionally diverges from OpenGL's traditional Global State Machine model. Instead, it adopts a modular, pipeline-oriented approach:
- Stateless over Global: Moving away from a single global state to isolated, reusable rendering objects.
- Customizable Pipelines: Different objects can be assigned unique pipelines (custom Vertex and Fragment shaders) on the fly.
## Build requirement
- vcpkg installed with path exposed as ${VCPKG_ROOT}
- libsdl2-dev
- ninja

`cmake --preset=default`
