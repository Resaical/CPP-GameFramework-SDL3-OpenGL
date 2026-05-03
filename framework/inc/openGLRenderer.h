#pragma once

#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "entitySystem.h"

class Camera;
class RenderStorage;

struct DebugLine
{
    glm::vec3 start = glm::vec3(0);
    glm::vec3 end = glm::vec3(0);
    glm::vec3 color = glm::vec3(1);
};

class OpenGLRenderer
{
public:

    void Init();
    GLuint CreateOpenGLProgram(const char* vs, const char* fs);

    void CreateLine(GLuint& VAO, GLuint& VBO);
    GLuint CreateTriangle();
    GLuint CreateQuad();
    GLuint CreateTexture(const char* path);

    void Render(SDL_Window* window, Camera* camera, RenderStorage* renderStorage);

    void DrawDebugLine(glm::vec3 a, glm::vec3 b, glm::vec3 color)
    {
        DebugLine dl;
        dl.start = a;
        dl.end = b;
        dl.color = color;
        debugLines.push_back(dl);
    }
    void DrawDebugBox(glm::vec2 center, glm::vec2 extents, glm::vec3 color);

    bool debugDraw = true;

private:

    void RenderLine(glm::vec3 a, glm::vec3 b, glm::vec3 color, glm::mat4 vp);

    GLuint program = 0;
    GLuint VAO = 0;

    GLuint lineVAO, lineVBO;
    GLuint lineProgram;

    std::vector<DebugLine> debugLines;
};