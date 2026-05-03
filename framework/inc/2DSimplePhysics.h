#pragma once
#include "entitySystem.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

class OpenGLRenderer;

enum ColliderType{ NONE, CIRCLE, BOX };

struct Overlaping
{
	Entity* entity;
	Entity* other;
};

class PhysicsSystem2D;

class Collider2D
{
public:
	Collider2D() {};
	~Collider2D() { delete colliderEntity; };

	void AddToPhysicsSystem(PhysicsSystem2D* physicsSystem);

	ColliderType GetType() { return type; }

	Entity* colliderEntity = nullptr;
	ColliderType type = NONE;
};

class CircleCollider2D : public Component, public Collider2D
{
public:

	CircleCollider2D() {};
	~CircleCollider2D() {};

	void Init() override 
	{ 
		colliderEntity = entity;
		type = CIRCLE; 
	}

	float radius = 32;
};

class BoxCollider2D : public Component, public Collider2D
{
public:

	BoxCollider2D() {};
	~BoxCollider2D() {};

	void Init() override
	{
		colliderEntity = entity;
		type = BOX;
	}

	void Update(float dt) override;
	void SetRendererForDebug(OpenGLRenderer* inOpenGLRenderer);	

	float halfWidth = 32;
	float halfHeight = 32;

	bool debug = true;
	glm::vec3 debugColor = glm::vec3(0, 1, 1);

private:
	OpenGLRenderer* openGLRenderer;
};

class Physics;

class PhysicsSystem2D
{
public:
	PhysicsSystem2D() {};
	~PhysicsSystem2D() 
	{
		colliders2D.clear();
		physicsEntities.clear();
	};

	void Update(float dt);

	std::vector<Overlaping> GetAllOverlapings(Collider2D* c1);

	bool AreColliders2DOverlaping(Collider2D* c1, Collider2D* c2);
	bool IsCircleCircleOverlap(CircleCollider2D* c1, CircleCollider2D* c2);
	bool IsBoxBoxOverlap(BoxCollider2D* b1, BoxCollider2D* b2);
	bool IsCircleBoxOverlap(CircleCollider2D* c, BoxCollider2D* b);

	std::vector<Collider2D*> colliders2D;
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


