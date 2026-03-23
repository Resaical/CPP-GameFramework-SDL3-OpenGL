#include "sdlApp.h"
#include <iostream>

void SDLApp::Init()
{
    SDL_Init(SDL_INIT_VIDEO);
    window = InitOpenGLWindow();

    // FrameTime
    prevFrameStart = std::chrono::high_resolution_clock::now();

}

void SDLApp::UpdateInput()
{
    SDL_Event event;
    events.clear();

    // Input
    while (SDL_PollEvent(&event))
    {
        events.push_back(event);
    }
}

void SDLApp::InitFrameTiming()
{
    // StartFrameTime
    frameStart = std::chrono::high_resolution_clock::now();
    deltaTime = std::chrono::duration<float>(frameStart - prevFrameStart).count();
    prevFrameStart = frameStart;
}

void SDLApp::EndFrameTiming()
{
    //EndFrameTime
    frameEnd = std::chrono::high_resolution_clock::now();
}

void SDLApp::WaitForTargetFramerate(float targetFrameTime)
{
    float frameTime = std::chrono::duration<float>(frameEnd - frameStart).count();

    if (frameTime < targetFrameTime)
    {
        SDL_Delay((targetFrameTime - frameTime) * 1000.0f);
    }

    std::cout << deltaTime << "\n";
    FameCount++;
}


void SDLApp::EndSession()
{
    SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_Window* SDLApp::InitOpenGLWindow()
{
    // OpenGL Window
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_Window* window = SDL_CreateWindow("Window", 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(window);

    return window;
}

bool SDLApp::KeyIsDown(unsigned int SDLK_Key)
{
    for (auto e : events)
    {        
        if (e.key.key == SDLK_Key)
        {
            if (e.type == SDL_EVENT_KEY_DOWN) return true; 
            return false;
        }        
    }
    return false;
}
