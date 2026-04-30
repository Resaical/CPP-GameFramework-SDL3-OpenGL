#include "game.h"
#include "entitySystem.h"
#include <basicComponents.h>
#include "openGLRenderer.h"
#include "sdlApp.h"
//#include "box2d.h"


void Game::Init()
{
	//// Box2D
	//bworld = new b2WorldDef();
	//b2CreateWorld(bworld);
	


	entitySystem = new EntitySystem();
	renderStorage = new RenderStorage();
	physicsSystem2D = new PhysicsSystem2D();

	// Camera
	auto camEntity = entitySystem->CreateEntity();
	auto camTransform = camEntity->AddComponent<Transform2D>();
	camera = camEntity->AddComponent<Camera>();

	// Floor
	{
		auto entity = entitySystem->CreateEntity();

		auto transform = entity->AddComponent<Transform2D>();
		transform->SetScale({ 8,1 });
		transform->SetWorldPosition({ 0,-256 });

		auto imageRenderer = entity->AddComponent<ImageRender>();
		imageRenderer->AddToRenderStorage(renderStorage);
		imageRenderer->texture = renderer->CreateTexture("assets/images/Floor.png");

		auto boxCollider2D = entity->AddComponent<BoxCollider2D>();
		boxCollider2D->AddToPhysicsSystem(physicsSystem2D);
		boxCollider2D->halfWidth = 32 * 8;
		boxCollider2D->halfHeight = 32;

		floor = entity;
	}

	// Floor 2
	{
		auto entity = entitySystem->CreateEntity();

		auto transform = entity->AddComponent<Transform2D>();
		transform->SetScale({ 8,1 });
		transform->SetWorldPosition({ 256,0 });

		auto imageRenderer = entity->AddComponent<ImageRender>();
		imageRenderer->AddToRenderStorage(renderStorage);
		imageRenderer->texture = renderer->CreateTexture("assets/images/Floor.png");

		auto boxCollider2D = entity->AddComponent<BoxCollider2D>();
		boxCollider2D->AddToPhysicsSystem(physicsSystem2D);
		boxCollider2D->halfWidth = 32 * 8;
		boxCollider2D->halfHeight = 32;

		floor = entity;
	}


	// Character
	{
		auto entity = entitySystem->CreateEntity();

		auto transform = entity->AddComponent<Transform2D>();
		transform->SetWorldPosition({ 0, 256 });

		auto imageRenderer = entity->AddComponent<ImageRender>();
		imageRenderer->AddToRenderStorage(renderStorage);
		imageRenderer->texture = renderer->CreateTexture("assets/images/testImage.png");

		auto boxCollider2D = entity->AddComponent<BoxCollider2D>();
		boxCollider2D->AddToPhysicsSystem(physicsSystem2D);
		boxCollider2D->halfWidth = 32;
		boxCollider2D->halfHeight = 32;

		auto physics = entity->AddComponent<Physics>();
		physics->AddToPhysicsSystem(physicsSystem2D);

		auto gravity = entity->AddComponent<Gravity>();

		auto collisionResponseBounce = entity->AddComponent<CollisionResolutionBounce>();
		collisionResponseBounce->AddPhysicsSystem2DReference(physicsSystem2D);

		character = entity;
	}


	// Init entities
	entitySystem->Init();
}

void Game::Update(float dt)
{
	// Physics
	physicsSystem2D->Update(dt);

	float jumpForce = 15000;
	float movementForce = 750;

	if (sdlApp->KeyIsDown(SDLK_SPACE))
	{
		auto physics = character->GetComponent<Physics>();
		physics->f += glm::vec2(0, jumpForce);
	}


	if (sdlApp->KeyIsDown(SDLK_LEFT))
	{
		auto physics = character->GetComponent<Physics>();
		physics->f += glm::vec2(-movementForce, 0);
	}

	if (sdlApp->KeyIsDown(SDLK_RIGHT))
	{
		auto physics = character->GetComponent<Physics>();
		physics->f += glm::vec2( movementForce, 0);
	}

	// Update entities
	entitySystem->Update(dt);


}