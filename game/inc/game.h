#pragma once

class EntitySystem;
class RenderStorage;
class OpenGLRenderer;
class Camera;
class PhysicsSystem2D;
class Entity;
class SDLApp;
class TileMap;
class FileLoader;
class SpriteSheet;

class Game
{
public:

	Game() {};
	~Game() 
	{
		delete entitySystem;
		delete renderStorage;
		delete tileMap;
	};

	void Init(FileLoader* fileLoader, OpenGLRenderer* openGLRendererIn);

	void Update(float dt);

	SDLApp* sdlApp;

	FileLoader* fileLoader;
	Camera* camera;
	OpenGLRenderer* renderer;
	EntitySystem* entitySystem;
	OpenGLRenderer* openGLRenderer;
	RenderStorage* renderStorage;
	PhysicsSystem2D* physicsSystem2D;
	TileMap* tileMap;
	SpriteSheet* spriteSheet0;

	Entity* box1;
	Entity* box2;
};