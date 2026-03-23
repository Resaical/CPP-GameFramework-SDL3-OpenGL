#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <chrono>


class SDLApp
{
public:

	SDLApp() {};
	~SDLApp() {};

	void Init();

	void UpdateInput();

	void InitFrameTiming();

	void EndFrameTiming();

	void WaitForTargetFramerate(float targetFrameTime);

	void EndSession();

	// Utility
	float GetDeltaTime() { return deltaTime; }

	int GetFrameCount() { return FameCount; }

	SDL_Window* InitOpenGLWindow();

	SDL_Window* GetWindow() { return window; };

	bool KeyIsDown(unsigned int SDL_Key);


private:

	SDL_Window* window;

	// Input
	std::vector<SDL_Event> events;

	std::chrono::high_resolution_clock::time_point prevFrameStart;
	std::chrono::high_resolution_clock::time_point frameStart;
	std::chrono::high_resolution_clock::time_point frameEnd;

	int FameCount = 0;
	float deltaTime = 0;

};