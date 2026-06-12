#pragma once
#include "entitySystem.h"
#include "glm/glm.hpp"

class TileMap;

class PlatformerMovement : public Component
{
public:

	PlatformerMovement() {};
	~PlatformerMovement() {};

	void SetInput(glm::vec2 inputIn) { input = inputIn; }
	glm::vec2 GetProcessedPosition(float dt, glm::vec2 positionIn);

	glm::vec2 GetCorrectedPosition(TileMap* tileMap, glm::vec2 prevPosition, glm::vec2 nextPosition);

	glm::vec2 input = { 0,0 };
	glm::vec2 velocity = { 0,0 };
	glm::vec2 gravity = { 0,-32 };

	float speed = 5;

	bool grounded = false;

};