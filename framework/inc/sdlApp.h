#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <chrono>

struct Key
{
	bool wasPressedDownThisFrame = false;
	bool IsDown = false;
	bool releasedThisFrame = false;
};


class SDLApp
{
public:

	SDLApp() {};
	~SDLApp() {};

	void Init();
	SDL_Window* InitOpenGLWindow();
	SDL_Window* GetWindow() { return window; };

	void InitFrameTiming();
	void WaitForTargetFramerate(float targetFrameTime);
	void EndFrameTiming();

	void EndSession();

	void UpdateInput();
	bool KeyWasPressedThisFrame(unsigned int SDL_SCANCODE_Key);
	bool KeyIsDown(unsigned int SDL_SCANCODE_Key);
	bool KeyReleasedThisFrame(unsigned int SDL_SCANCODE_Key);

	// Utility
	float GetDeltaTime() { return deltaTime; }
	int GetFrameCount() { return FameCount; }

private:

	SDL_Window* window;

	// Input
	std::vector<SDL_Event> events;

	std::chrono::high_resolution_clock::time_point prevFrameStart;
	std::chrono::high_resolution_clock::time_point frameStart;
	std::chrono::high_resolution_clock::time_point frameEnd;

	int FameCount = 0;
	float deltaTime = 0;

	Key* keys = nullptr;
};