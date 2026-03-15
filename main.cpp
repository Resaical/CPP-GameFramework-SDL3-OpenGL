#include <iostream>
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <chrono>

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow( "Window", 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(window);
    gladLoadGL();

    SDL_Event event;    
    auto prevFrameStart = std::chrono::high_resolution_clock::now();       
    bool run = true;
    const float frameRate = 1.0f / 60.0f;
    int FameCount = 0;

    while (run)
    {
        // Start
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
        glm::vec4 backgroundColor(1, 1 - FameCount * 0.01f, FameCount * 0.01f, 1);

        glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, backgroundColor.w);
        glClear(GL_COLOR_BUFFER_BIT);

        SDL_GL_SwapWindow(window);


        //End
        auto frameEnd = std::chrono::high_resolution_clock::now();
        float frameTime = std::chrono::duration<float>(frameEnd - currentFrameStart).count();

        if (frameTime < frameRate)
        {
            SDL_Delay((frameRate - frameTime) * 1000.0f);
        }

        std::cout << deltaTime << "\n";
        FameCount++;
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
}