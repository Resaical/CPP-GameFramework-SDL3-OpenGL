# CMake Version
cmake_minimum_required(VERSION 3.15...4.2.3)

set(SDL_TEST_LIBRARY OFF CACHE BOOL "Build the SDL3_test library")
set(SDL_DISABLE_UNINSTALL ON CACHE BOOL "Disable uninstallation of SDL3")
set(INPUT_USE_SDL3 ON CACHE BOOL "Use SDL3 for input handling")

add_subdirectory(SDL)
add_subdirectory(glm)