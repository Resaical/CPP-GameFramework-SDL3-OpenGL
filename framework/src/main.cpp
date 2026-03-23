#include <iostream>
#include <glm/glm.hpp>
#include "sdlApp.h"
#include "openGLRenderer.h"
#include "fileLoader.h"
#include "game.h"

int main()
{
    SDLApp sdlApp; // Window and Input
    sdlApp.Init();

    OpenGLRenderer renderer;
    renderer.Init();

    Game game;
    game.Init();

    float targetFrameTime = 1.0f / 60.0f;
    bool run = true;
    while (run)
    {
        // Frame time init
        sdlApp.InitFrameTiming();
        float dt = sdlApp.GetDeltaTime();
        int frameCount = sdlApp.GetFrameCount();

        // Input
        sdlApp.UpdateInput();

        if (sdlApp.KeyIsDown(SDLK_Q)) run = false;
        
        // Update
        game.Update(dt);

        // Render
        auto window = sdlApp.GetWindow();
        renderer.Render(window, frameCount);

        // Frame time end
        sdlApp.EndFrameTiming();
        sdlApp.WaitForTargetFramerate(targetFrameTime);
    }

    // Close App
    sdlApp.EndSession();
}