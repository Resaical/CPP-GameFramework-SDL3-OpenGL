#include <iostream>
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <chrono>

#include "fileLoader.h"

FileLoader fileLoader;

SDL_Window* InitOpenGLWindow()
{
    // OpenGL Window
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_Window* window = SDL_CreateWindow("Window", 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(window);
    gladLoadGL();

    glViewport(0, 0, 800, 600);

    return window;
}

GLuint CreateOpenGLProgram(const char* vs, const char* fs)
{
    GLuint v = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(v, 1, &vs, nullptr);
    glCompileShader(v);

    GLuint f = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(f, 1, &fs, nullptr);
    glCompileShader(f);

    GLuint program = glCreateProgram();
    glAttachShader(program, v);
    glAttachShader(program, f);
    glLinkProgram(program);

    glDeleteShader(v);
    glDeleteShader(f);

    return program;
}

GLuint CreateTriangle()
{
    // Load Triangle for rendering
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    return VAO;
}

GLuint CreateQuad()
{
    float vertices[] = {
        // pos              // uv
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, 0.0f,  1.0f, 1.0f,

        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
         0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,  0.0f, 1.0f
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    return VAO;
}

GLuint CreateTexture(const char* path)
{
    int w, h, channels;
    auto data = fileLoader.LoadImageData(path, w, h, channels);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
    GLenum internalFormat = (channels == 4) ? GL_SRGB_ALPHA : GL_SRGB;

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, format, GL_UNSIGNED_BYTE, data);

    fileLoader.ReleaseImageData(data);

    return texture;
}


void Render(SDL_Window* window, GLuint program, GLuint VAO, GLuint texture, float FameCount)
{
    glm::vec4 backgroundColor(1, 1 - FameCount * 0.01f, FameCount * 0.01f, 1);

    glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, backgroundColor.w);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);

    glUniform1i(glGetUniformLocation(program, "tex"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    SDL_GL_SwapWindow(window);
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = InitOpenGLWindow();

    auto vs = fileLoader.LoadTextFileInString("assets/shaders/textureQuadVertex.glsl");
    auto fs = fileLoader.LoadTextFileInString("assets/shaders/textureQuadFragment.glsl");

    GLuint program = CreateOpenGLProgram(vs.c_str(), fs.c_str());
    GLuint VAO = CreateQuad();
    GLuint texture = CreateTexture("assets/images/testImage.png");

    // FrameTime
    auto prevFrameStart = std::chrono::high_resolution_clock::now();       
    bool run = true;
    const float frameRate = 1.0f / 60.0f;
    int FameCount = 0;

    // Input
    SDL_Event event;    

    while (run)
    {
        // StartFrameTime
        auto currentFrameStart = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentFrameStart - prevFrameStart).count();
        prevFrameStart = currentFrameStart;

        // Input
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_KEY_DOWN)
            {
                if (event.key.key == SDLK_Q) run = false;
            }
        }

        // Update


        // Render
        Render(window, program, VAO, texture, FameCount);

        //EndFrameTime
        auto frameEnd = std::chrono::high_resolution_clock::now();
        float frameTime = std::chrono::duration<float>(frameEnd - currentFrameStart).count();

        if (frameTime < frameRate)
        {
            SDL_Delay((frameRate - frameTime) * 1000.0f);
        }

        std::cout << deltaTime << "\n";
        FameCount++;
    }

    // Close App
    SDL_DestroyWindow(window);
    SDL_Quit();
}