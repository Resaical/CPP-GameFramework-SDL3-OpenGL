#include "platformerMovement.h"
#include <tileMap.h>

glm::vec2 PlatformerMovement::GetProcessedPosition(float dt, glm::vec2 positionIn)
{
	auto deltaFallVelocity = gravity * dt;
	velocity += deltaFallVelocity;
	velocity.x = input.x * speed;

	auto dp = velocity * dt;
	auto np = positionIn + dp;

	return np;
}

glm::vec2 PlatformerMovement::GetCorrectedPosition(TileMap* tileMap, glm::vec2 prevPosition, glm::vec2 nextPosition)
{
	glm::vec2 finalPosition = nextPosition;

	glm::vec2 nextXPosition = { nextPosition.x, prevPosition.y };
	glm::vec2 nextYPosition = { prevPosition.x, nextPosition.y };

	int xIndex = tileMap->GetTileIndexAtPosition(nextXPosition);
	int yIndex = tileMap->GetTileIndexAtPosition(nextYPosition);

	if (xIndex != -1)
	{
		finalPosition.x = prevPosition.x;
		velocity.x = 0;
	}
	if (yIndex != -1)
	{
		finalPosition.y = prevPosition.y;
		velocity.y = 0;
	}
	
	return finalPosition;
}
