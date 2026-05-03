#pragma once

class EntitySystem;
class RenderStorage;
class OpenGLRenderer;
class Camera;
class PhysicsSystem2D;
class Entity;
class SDLApp;

struct b2WorldDef;

class Game
{
public:

	Game() {};
	~Game() 
	{
		delete entitySystem;
		delete renderStorage;
		delete bworld;
	};

	void Init();

	void Update(float dt);

	SDLApp* sdlApp;

	b2WorldDef* bworld = nullptr;


	Camera* camera;
	OpenGLRenderer* renderer;
	EntitySystem* entitySystem;
	RenderStorage* renderStorage;
	PhysicsSystem2D* physicsSystem2D;

	Entity* box1;
	Entity* box2;
};