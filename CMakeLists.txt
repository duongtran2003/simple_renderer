cmake_minimum_required(VERSION 3.15) # 3.15+ is better for modern vcpkg support

project(SimpleRasterizer LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(glm CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

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
