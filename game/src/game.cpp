#include "game.h"
#include "entitySystem.h"
#include <basicComponents.h>
#include "openGLRenderer.h"
#include "sdlApp.h"
#include "2DSimplePhysics.h"


void Game::Init()
{
	entitySystem = new EntitySystem();
	renderStorage = new RenderStorage();
	physicsSystem2D = new PhysicsSystem2D();

	// Camera
	auto camEntity = entitySystem->CreateEntity();
	auto camTransform = camEntity->AddComponent<Transform2D>();
	camera = camEntity->AddComponent<Camera>();

	// Box 1
	{
		auto entity = entitySystem->CreateEntity();
		box1 = entity;

		auto transform = entity->AddComponent<Transform2D>();
		transform->SetWorldPosition({ 0, 0 });

		//auto imageRenderer = entity->AddComponent<ImageRender>();
		//imageRenderer->AddToRenderStorage(renderStorage);
		//imageRenderer->texture = renderer->CreateTexture("assets/images/testImage.png");

		auto boxCollider2D = entity->AddComponent<BoxCollider2D>();
		boxCollider2D->AddToPhysicsSystem(physicsSystem2D);
		boxCollider2D->SetRendererForDebug(renderer);
		boxCollider2D->halfWidth = 32;
		boxCollider2D->halfHeight = 32;
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
	// Physics
	physicsSystem2D->Update(dt);

	auto t1 = box1->GetComponent<Transform2D>();
	auto t2 = box2->GetComponent<Transform2D>();
	auto b1 = box1->GetComponent<BoxCollider2D>();
	auto b2 = box2->GetComponent<BoxCollider2D>();

	float speed = 2;
	if (sdlApp->KeyIsDown(SDL_SCANCODE_LEFT))
	{
		auto currentPos = t1->GetWorldPosition();
		auto newPos = currentPos - glm::vec2(speed,0);
		t1->SetWorldPosition(newPos);
	}
	if (sdlApp->KeyIsDown(SDL_SCANCODE_RIGHT))
	{
		auto currentPos = t1->GetWorldPosition();
		auto newPos = currentPos + glm::vec2(speed, 0);
		t1->SetWorldPosition(newPos);

	}
	if (sdlApp->KeyIsDown(SDL_SCANCODE_UP))
	{
		auto currentPos = t1->GetWorldPosition();
		auto newPos = currentPos + glm::vec2(0, speed);
		t1->SetWorldPosition(newPos);
	}
	if (sdlApp->KeyIsDown(SDL_SCANCODE_DOWN))
	{
		auto currentPos = t1->GetWorldPosition();
		auto newPos = currentPos - glm::vec2(0, speed);
		t1->SetWorldPosition(newPos);

	}
	if (sdlApp->KeyWasPressedThisFrame(SDL_SCANCODE_SPACE))
	{
	}

	bool overlap = false;
	auto overlapings = physicsSystem2D->GetAllOverlapings(b1);

	if (!overlapings.empty()) overlap = true;
	
	if (overlap) b1->debugColor = glm::vec3(1, 0, 0);
	else b1->debugColor = glm::vec3(0, 1, 0);

	// Update entities
	entitySystem->Update(dt);


}