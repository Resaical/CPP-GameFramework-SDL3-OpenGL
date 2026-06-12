#include "sdlApp.h"
#include <iostream>

void SDLApp::Init()
{
    SDL_Init(SDL_INIT_VIDEO);
    window = InitOpenGLWindow();

    // FrameTime
    prevFrameStart = std::chrono::high_resolution_clock::now();

    SDL_PumpEvents();
    int numberKeys = 0;
    const bool* SDL_keys = SDL_GetKeyboardState(&numberKeys);
    keys = new Key[numberKeys];
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

void SDLApp::InitFrameTiming()
{
    // StartFrameTime
    frameStart = std::chrono::high_resolution_clock::now();
    deltaTime = std::chrono::duration<float>(frameStart - prevFrameStart).count();
    prevFrameStart = frameStart;
}

void SDLApp::WaitForTargetFramerate(float targetFrameTime)
{
    float frameTime = std::chrono::duration<float>(frameEnd - frameStart).count();

    if (frameTime < targetFrameTime)
    {
        SDL_Delay((targetFrameTime - frameTime) * 1000.0f);
    }
    FameCount++;
}

void SDLApp::EndFrameTiming()
{
    //EndFrameTime
    frameEnd = std::chrono::high_resolution_clock::now();
}

void SDLApp::EndSession()
{
    SDL_DestroyWindow(window);
    SDL_Quit();
}


void SDLApp::UpdateInput()
{
    SDL_PumpEvents();
    int numberKeys = 0;
    const bool* SDL_keys = SDL_GetKeyboardState(&numberKeys);

    for (int i = 0; i < numberKeys; i++)
    {
        keys[i].wasPressedDownThisFrame = false;
        keys[i].releasedThisFrame = false;

        if (!keys[i].IsDown && !SDL_keys[i]) continue;
        if (keys[i].IsDown && SDL_keys[i]) continue;

        keys[i].IsDown = SDL_keys[i];

        if (SDL_keys[i]) keys[i].wasPressedDownThisFrame = true;
        else keys[i].releasedThisFrame = true;        
    }
}

bool SDLApp::KeyWasPressedThisFrame(unsigned int SDL_SCANCODE_Key)
{
    return keys[SDL_SCANCODE_Key].wasPressedDownThisFrame;
}

bool SDLApp::KeyIsDown(unsigned int SDL_SCANCODE_Key)
{
    return keys[SDL_SCANCODE_Key].IsDown;
}

bool SDLApp::KeyReleasedThisFrame(unsigned int SDL_SCANCODE_Key)
{
    return keys[SDL_SCANCODE_Key].releasedThisFrame;
}
