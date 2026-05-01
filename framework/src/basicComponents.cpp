#include "basicComponents.h"

glm::vec2 Transform2D::GetWorldPosition()
{
	return glm::vec2(model2D[2][0], model2D[2][1]);
}

float Transform2D::GetWorldRotation()
{
	return atan2(model2D[0][1], model2D[0][0]);
}

glm::vec2 Transform2D::GetScale()
{
	float scaleX = glm::length(glm::vec2(model2D[0][0], model2D[0][1]));
	float scaleY = glm::length(glm::vec2(model2D[1][0], model2D[1][1]));
	return glm::vec2(scaleX, scaleY);
}

void Transform2D::SetWorldPosition(glm::vec2 position)
{
	model2D[2][0] = position.x; 
	model2D[2][1] = position.y;
}

void Transform2D::SetWorldRotation(float rotation)
{
	glm::vec2 scale = GetScale();

	float cosA = cos(rotation);
	float sinA = sin(rotation);

	model2D[0][0] = cosA * scale.x;
	model2D[0][1] = sinA * scale.x;

	model2D[1][0] = -sinA * scale.y;
	model2D[1][1] = cosA * scale.y;
}

void Transform2D::SetScale(glm::vec2 scale)
{
	float rotation = GetWorldRotation();

	float cosA = cos(rotation);
	float sinA = sin(rotation);

	model2D[0][0] = cosA * scale.x;
	model2D[0][1] = sinA * scale.x;

	model2D[1][0] = -sinA * scale.y;
	model2D[1][1] = cosA * scale.y;
}

void ImageRender::AddToRenderStorage(RenderStorage* renderStorage)
{
	renderStorage->imageRenders.push_back(this);
}

