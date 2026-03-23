#pragma once

#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <glad/glad.h>


class OpenGLRenderer
{
public:

    void Init();
    GLuint CreateOpenGLProgram(const char* vs, const char* fs);

    GLuint CreateTriangle();
    GLuint CreateQuad();
    GLuint CreateTexture(const char* path);

    void Render(SDL_Window* window, float FameCount);

private:

    GLuint program = 0;
    GLuint VAO = 0;
    GLuint texture = 0;
};