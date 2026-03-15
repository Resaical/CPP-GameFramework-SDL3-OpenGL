cmake_minimum_required(VERSION 3.15...4.2.3)

project(CppGameFramework)
add_executable(CppGameFramework main.cpp)

# glm
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE externals/glm)

# SDL3
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
add_subdirectory(externals/SDL)
target_link_libraries(CppGameFramework PRIVATE glm SDL3::SDL3-static)

# GLAD (manual)
add_library(glad STATIC externals/glad/src/glad.c)
target_include_directories(glad PUBLIC externals/glad/include)
target_link_libraries(CppGameFramework PRIVATE glad)

# OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(CppGameFramework PRIVATE OpenGL::GL)