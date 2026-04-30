#pragma once

#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "entitySystem.h"

class Camera;
class RenderStorage;

class OpenGLRenderer
{
public:

    void Init();
    GLuint CreateOpenGLProgram(const char* vs, const char* fs);

    GLuint CreateTriangle();
    GLuint CreateQuad();
    GLuint CreateTexture(const char* path);

    void Render(SDL_Window* window, Camera* camera, RenderStorage* renderStorage);

private:

    GLuint program = 0;
    GLuint VAO = 0;
};