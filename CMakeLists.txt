cmake_minimum_required(VERSION 3.15) # 3.15+ is better for modern vcpkg support

project(SimpleRenderer LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(glm CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)

add_executable(${PROJECT_NAME} 
    src/main.cpp 
    src/window.cpp
    src/app.cpp
    src/input.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    src
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${GLM_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    glm::glm-header-only
    ${SDL2_LIBRARIES}
)
