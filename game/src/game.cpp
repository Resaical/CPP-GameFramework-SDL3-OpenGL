#include "game.h"
#include "entitySystem.h"

void Game::Init()
{
	entitySystem = new EntitySystem();

	//Add entities and components
	auto testEntity = entitySystem->CreateEntity();
	auto testEntityComponent = testEntity->AddComponent<Component>();

	// Init entities
	entitySystem->Init();
}

void Game::Update(float dt)
{
	// Update entities
	entitySystem->Update(dt);
}