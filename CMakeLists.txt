cmake_minimum_required(VERSION 3.15...4.2.3)

project(CppGameFramework)

set(SOURCES
    src/main.cpp
    src/fileLoader.cpp
)

set(HEADERS
    inc/fileLoader.h
)

add_executable(CppGameFramework
    ${SOURCES}
    ${HEADERS}
)
target_include_directories(CppGameFramework PRIVATE inc)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# glm
target_include_directories(CppGameFramework PRIVATE externals/glm)

# stb
target_include_directories(CppGameFramework PRIVATE externals/stb)

# SDL3
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
add_subdirectory(externals/SDL)
target_link_libraries(CppGameFramework PRIVATE SDL3::SDL3-static)

# GLAD (manual)
add_library(glad STATIC externals/glad/src/glad.c)
target_include_directories(glad PUBLIC externals/glad/include)
target_link_libraries(CppGameFramework PRIVATE glad)

# OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(CppGameFramework PRIVATE OpenGL::GL)