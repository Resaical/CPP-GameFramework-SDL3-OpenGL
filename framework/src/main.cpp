#include <iostream>
#include <glm/glm.hpp>
#include "sdlApp.h"
#include "openGLRenderer.h"
#include "game.h"
#include <fileLoader.h>

int main()
{
    SDLApp sdlApp; // Window and Input
    sdlApp.Init();

    FileLoader fileLoader;

    OpenGLRenderer renderer;
    renderer.Init(&fileLoader);

    Game game;
    game.sdlApp = &sdlApp;
    game.renderer = &renderer;
    game.Init(&fileLoader, &renderer);    

    float targetFrameTime = 1.0f / 60.0f;
    bool run = true;
    while (run)
    {
        // Frame time init
        sdlApp.InitFrameTiming();
        float dt = sdlApp.GetDeltaTime();

        // Input
        sdlApp.UpdateInput();

        if (sdlApp.KeyIsDown(SDLK_Q)) run = false;
        
        renderer.RenderBackground();

        // Update
        game.Update(dt);

        // Render
        auto window = sdlApp.GetWindow();
        renderer.Render(window, game.camera, game.renderStorage);
        SDL_GL_SwapWindow(window);
        

        // Frame time end
        sdlApp.EndFrameTiming();
        sdlApp.WaitForTargetFramerate(targetFrameTime);
    }

    // Close App
    sdlApp.EndSession();
}