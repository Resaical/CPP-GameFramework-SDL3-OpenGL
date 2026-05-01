#pragma once
#include "entitySystem.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

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


