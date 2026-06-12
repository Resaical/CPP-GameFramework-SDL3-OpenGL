#include "game.h"
#include "entitySystem.h"
#include <basicComponents.h>
#include "openGLRenderer.h"
#include "sdlApp.h"
#include "2DSimplePhysics.h"
#include "tileMap.h"
#include <fileLoader.h>
#include "spriteSheet.h"
#include <platformerMovement.h>


void Game::Init(FileLoader* fileLoaderIn, OpenGLRenderer* openGLRendererIn)
{
	fileLoader = fileLoaderIn;	
	openGLRenderer = openGLRendererIn;

	entitySystem = new EntitySystem();
	renderStorage = new RenderStorage();
	physicsSystem2D = new PhysicsSystem2D();
	tileMap = new TileMap();
	spriteSheet0 = new SpriteSheet();


	// SpriteSheet
	spriteSheet0->InitRenderProgram(openGLRendererIn);
	spriteSheet0->LoadSpriteSheet(openGLRendererIn, "assets/images/Tileset_2D Platformer.png", 16);

	// TileMap
	auto data = fileLoader->LoadTextFileInString("assets/csv/TestMap.csv");
	tileMap->AddTileToIgnore(56);
	tileMap->LoadTileMap(data);
	tileMap->SetSpriteSheet(spriteSheet0);

	// Camera
	auto camEntity = entitySystem->CreateEntity();
	auto camTransform = camEntity->AddComponent<Transform2D>();
	camera = camEntity->AddComponent<Camera>();
	camTransform->SetWorldPosition({ 256,256 });

	// Box 1
	{
		auto entity = entitySystem->CreateEntity();
		box1 = entity;

		auto transform = entity->AddComponent<Transform2D>();
		transform->SetWorldPosition({ 64, 512 });

		auto imageRenderer = entity->AddComponent<ImageRender>();
		imageRenderer->AddToRenderStorage(renderStorage);
		imageRenderer->texture = renderer->CreateTexture("assets/images/testImage.png");

		auto platformerMovement = entity->AddComponent<PlatformerMovement>();
		platformerMovement->SetUpdateGroup(0);	
		platformerMovement->speed = 500;
		platformerMovement->gravity = { 0, -1000 };
	}

	for (int i = 0; i < 20; i++)
	{
		// Box 2
		{
			auto entity = entitySystem->CreateEntity();
			box2 = entity;

			auto transform = entity->AddComponent<Transform2D>();
			transform->SetWorldPosition({ (rand() % 800) - 400, (rand() % 600) - 300 });

			//auto imageRenderer = entity->AddComponent<ImageRender>();
			//imageRenderer->AddToRenderStorage(renderStorage);
			//imageRenderer->texture = renderer->CreateTexture("assets/images/testImage.png");

			auto boxCollider2D = entity->AddComponent<BoxCollider2D>();
			boxCollider2D->AddToPhysicsSystem(physicsSystem2D);
			boxCollider2D->SetRendererForDebug(renderer);
			boxCollider2D->halfWidth = 32;
			boxCollider2D->halfHeight = 32;
		}
	}

	// Init entities
	entitySystem->Init();
}

void Game::Update(float dt)
{
	auto bt = box1->GetComponent<Transform2D>();
	auto platformerMovement = box1->GetComponent<PlatformerMovement>();

	// Camera
	float speed = 2;
	glm::vec2 dp(0);

	if (sdlApp->KeyIsDown(SDL_SCANCODE_A)) dp = { -1,0 };
	if (sdlApp->KeyIsDown(SDL_SCANCODE_D))dp = { 1,0 };
	if (sdlApp->KeyIsDown(SDL_SCANCODE_W))dp = { 0,1 };
	if (sdlApp->KeyIsDown(SDL_SCANCODE_S))dp = { 0,-1 };

	auto ct = camera->entity->GetComponent<Transform2D>();
	auto cp = ct->GetWorldPosition();
	auto ncp = cp + dp * speed;
	ct->SetWorldPosition(ncp);

	// Player
	speed = 500;
	dp = { 0,0 };

	if (sdlApp->KeyIsDown(SDL_SCANCODE_LEFT)) dp -= glm::vec2(1,0);
	if (sdlApp->KeyIsDown(SDL_SCANCODE_RIGHT))dp += glm::vec2(1, 0);
	if (sdlApp->KeyIsDown(SDL_SCANCODE_UP)) dp += glm::vec2(0, 0);
	if (sdlApp->KeyIsDown(SDL_SCANCODE_DOWN))dp -= glm::vec2(0, 0);
	if (sdlApp->KeyWasPressedThisFrame(SDL_SCANCODE_SPACE))
	{
 		platformerMovement->velocity += glm::vec2(0, 500);
	}


	platformerMovement->SetInput(dp);

	entitySystem->UpdateGroup(dt, 0);

	auto currentPosition = bt->GetWorldPosition();
	auto newPosition = platformerMovement->GetProcessedPosition(dt, currentPosition);
	auto correctedPosition = platformerMovement->GetCorrectedPosition(tileMap, currentPosition, newPosition);

	bt->SetWorldPosition(correctedPosition);

	// Physics
	physicsSystem2D->Update(dt);

	// Update entities
	entitySystem->UpdateAllGroupsLeft(dt);

	tileMap->Render(openGLRenderer, camera);
}