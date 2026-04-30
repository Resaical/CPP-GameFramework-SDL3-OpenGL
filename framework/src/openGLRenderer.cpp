#include "openGLRenderer.h"
#include "fileLoader.h"
#include "basicComponents.h"
#include <glm/gtc/type_ptr.hpp>


FileLoader fileLoader;

void OpenGLRenderer::Init()
{
    gladLoadGL();
    glViewport(0, 0, 800, 600);

    auto vs = fileLoader.LoadTextFileInString("assets/shaders/basic2DVertex.glsl");
    auto fs = fileLoader.LoadTextFileInString("assets/shaders/basic2DFragment.glsl");
    program = CreateOpenGLProgram(vs.c_str(), fs.c_str());

    VAO = CreateQuad();
}

GLuint OpenGLRenderer::CreateOpenGLProgram(const char* vs, const char* fs)
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

GLuint OpenGLRenderer::CreateTriangle()
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

GLuint OpenGLRenderer::CreateQuad()
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

    
    for (auto& v : vertices)
    {
        if (v == 0.0f || v == 1.0f) continue;
        v *= 64.0f;
    }

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

GLuint OpenGLRenderer::CreateTexture(const char* path)
{
    int w, h, channels;
    auto data = fileLoader.LoadImageData(path, w, h, channels);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
    GLenum internalFormat = (channels == 4) ? GL_SRGB_ALPHA : GL_SRGB;

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, format, GL_UNSIGNED_BYTE, data);

    fileLoader.ReleaseImageData(data);

    return texture;
}

glm::mat4 Mat3ToMat4(const glm::mat3& m)
{
    glm::mat4 result(1.0f);

    result[0][0] = m[0][0];
    result[0][1] = m[0][1];

    result[1][0] = m[1][0];
    result[1][1] = m[1][1];

    result[3][0] = m[2][0]; // tx
    result[3][1] = m[2][1]; // ty

    return result;
}

void OpenGLRenderer::Render(SDL_Window* window, Camera* camera, RenderStorage* renderStorage)
{
    glm::vec4 backgroundColor(0,0,0.25f,1);

    glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, backgroundColor.w);
    glClear(GL_COLOR_BUFFER_BIT);

    auto camEntity = camera->entity;
    Transform2D* camTransform = camEntity->GetComponent<Transform2D>();

    glm::mat3 view = camera->GetViewMatrix(camTransform->GetWorldPosition(), camTransform->GetWorldRotation());
    glm::mat4 proj = camera->GetProjectionMatrix();

    for (auto& ir : renderStorage->imageRenders)
    {
        auto e = ir->entity;
        Transform2D* t = e->GetComponent<Transform2D>();
        auto model = t->model2D;

        glUseProgram(program);

        glm::mat4 vp = Mat3ToMat4(view * model);
        glm::mat4 mvp = proj * vp;

        GLint mvpLoc = glGetUniformLocation(program, "mvp");
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

        glUniform1i(glGetUniformLocation(program, "tex"), 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ir->texture);

        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    SDL_GL_SwapWindow(window);
}
