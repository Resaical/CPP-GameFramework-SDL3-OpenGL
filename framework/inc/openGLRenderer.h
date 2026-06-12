#pragma once

#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "entitySystem.h"

class Camera;
class RenderStorage;
class FileLoader;

struct DebugLine
{
    glm::vec3 start = glm::vec3(0);
    glm::vec3 end = glm::vec3(0);
    glm::vec3 color = glm::vec3(1);
};

class OpenGLRenderer
{
public:

    void Init(FileLoader* fileLoaderIn);
    GLuint CreateOpenGLProgram(const char* vs, const char* fs);

    void CreateLine(GLuint& VAO, GLuint& VBO);
    GLuint CreateTriangle();
    GLuint CreateQuad();
    GLuint CreateTexture(const char* path);
    GLuint CreateTexture(const char* path, int& pixelWidth, int& pixelHeight);


    void Render(SDL_Window* window, Camera* camera, RenderStorage* renderStorage);
    void RenderBackground();

    void DrawDebugLine(glm::vec3 a, glm::vec3 b, glm::vec3 color)
    {
        DebugLine dl;
        dl.start = a;
        dl.end = b;
        dl.color = color;
        debugLines.push_back(dl);
    }
    void DrawDebugBox(glm::vec2 center, glm::vec2 extents, glm::vec3 color);

    glm::mat4 Mat3ToMat4(const glm::mat3& m);

    bool debugDraw = true;
    FileLoader* fileLoader;
    GLuint VAO = 0;

private:

    void RenderLine(glm::vec3 a, glm::vec3 b, glm::vec3 color, glm::mat4 vp);


    GLuint program = 0;

    GLuint lineVAO, lineVBO;
    GLuint lineProgram;

    std::vector<DebugLine> debugLines;
};