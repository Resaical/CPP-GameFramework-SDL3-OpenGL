#pragma once
#include "entitySystem.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

class Transform2D : public Component
{
public:

	Transform2D() {};
	~Transform2D() {};

	glm::vec2 GetWorldPosition();
	float GetWorldRotation();
	glm::vec2 GetScale();

	void SetWorldPosition(glm::vec2 position);
	void SetWorldRotation(float rotation);
	void SetScale(glm::vec2 scale);

	glm::mat3 model2D = glm::mat3(1);
};

class Camera : public Component
{
public:

	Camera(){};
	~Camera() {};

	glm::mat3 GetViewMatrix(glm::vec2 position, float rotation)
	{
		float c = cos(-rotation);
		float s = sin(-rotation);

		glm::mat3 view(1.0f);

		view[0][0] = c;
		view[0][1] = s;
		view[1][0] = -s;
		view[1][1] = c;

		view[2][0] = -position.x;
		view[2][1] = -position.y;

		return view;
	}

	glm::mat4 GetProjectionMatrix()
	{
		auto halfWidth = width * 0.5f;
		auto halfHeight = height * 0.5f;

		glm::mat4 proj = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight);
		return proj;
	}

	float width = 800;
	float height = 600;
	float near = -1;
	float far = 1;
};

class RenderStorage;

class ImageRender : public Component
{
public:

	ImageRender() {};
	~ImageRender() {};

	void AddToRenderStorage(RenderStorage* renderStorage);

	GLuint texture;
};

class RenderStorage
{
public:

	RenderStorage() {};
	~RenderStorage() {};

	std::vector<ImageRender*> imageRenders;
};

struct Collision
{
	Entity* entity;
	Entity* other;

};

class PhysicsSystem2D;

class BoxCollider2D : public Component
{
public:

	BoxCollider2D() {};
	~BoxCollider2D() {};

	void AddToPhysicsSystem(PhysicsSystem2D* physicsSystem);
	bool IsCollidingThisFrame(PhysicsSystem2D* physicsSystem, Collision& collisio);

	float halfWidth = 32;
	float halfHeight = 32;

};

class Physics;

class PhysicsSystem2D
{
public:
	PhysicsSystem2D() {};
	~PhysicsSystem2D() 
	{
		boxColliders2D.clear();
		physicsEntities.clear();
	};

	void Update(float dt);

	std::vector<BoxCollider2D*> boxColliders2D;
	std::vector<Physics*> physicsEntities;
};

class Physics : public Component
{
public:

	Physics() {};
	~Physics() {};

	void AddToPhysicsSystem(PhysicsSystem2D* physicsSystem);

	void UpdatePhysics(float dt);

	glm::vec2 f = glm::vec2(0);
	glm::vec2 v = glm::vec2(0);
};

class Gravity : public Component
{
public:

	Gravity() {};
	~Gravity() {};

	void Update(float dt) override;

	glm::vec2 force = { 0, -256 };
};

class CollisionResolutionBounce : public Component
{
public:

	CollisionResolutionBounce() {};
	~CollisionResolutionBounce() {};

	void AddPhysicsSystem2DReference(PhysicsSystem2D* reference)
	{
		physicsSystem2D = reference;
	}
	void Update(float dt) override;

	PhysicsSystem2D* physicsSystem2D;
	float bounceFactor = 0.6f;
};


