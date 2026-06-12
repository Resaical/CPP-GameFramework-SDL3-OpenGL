#include "2DSimplePhysics.h"
#include "openGLRenderer.h"
#include <basicComponents.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/component_wise.hpp>
#include <glm/gtx/norm.hpp>
#include <unordered_map>



void BoxCollider2D::Update(float dt)
{
	if (!debug) return;

	auto t = entity->GetComponent<Transform2D>();
	openGLRenderer->DrawDebugBox(t->GetWorldPosition(), glm::vec2(halfWidth, halfHeight), debugColor);
}

void BoxCollider2D::SetRendererForDebug(OpenGLRenderer* inOpenGLRenderer)
{
	openGLRenderer = inOpenGLRenderer;
}


void PhysicsSystem2D::Update(float dt)
{
	for (auto p : physicsEntities)
	{
		p->UpdatePhysics(dt);
	}
}

std::vector<Overlaping> PhysicsSystem2D::GetAllOverlapings(Collider2D* c1)
{
	std::vector<Overlaping> overlapings;

	for (auto c2 : colliders2D)
	{
		if (c2 == c1) continue;

		auto overlaping = AreColliders2DOverlaping(c1, c2);

		if (!overlaping) continue;

		Overlaping col;
		col.entity = c1->colliderEntity;
		col.other = c2->colliderEntity;
		overlapings.push_back(col);
	}

	return overlapings;
}

bool PhysicsSystem2D::AreColliders2DOverlaping(Collider2D* c1, Collider2D* c2)
{
	bool overlaping = false;

	auto type1 = c1->GetType();
	auto type2 = c2->GetType();

	if (type1 == CIRCLE && type2 == CIRCLE)
	{
		auto cc1 = static_cast<CircleCollider2D*>(c1);
		auto cc2 = static_cast<CircleCollider2D*>(c2);
		overlaping = IsCircleCircleOverlap(cc1, cc2);
	}
	if (type1 == BOX && type2 == BOX)
	{
		auto bc1 = static_cast<BoxCollider2D*>(c1);
		auto bc2 = static_cast<BoxCollider2D*>(c2);
		overlaping = IsBoxBoxOverlap(bc1, bc2);
	}
	if (type1 == BOX && type2 == CIRCLE)
	{
		auto bc1 = static_cast<BoxCollider2D*>(c1);
		auto cc2 = static_cast<CircleCollider2D*>(c2);
		overlaping = IsCircleBoxOverlap(cc2, bc1);
	}
	if (type1 == CIRCLE && type2 == BOX)
	{
		auto cc1 = static_cast<CircleCollider2D*>(c1);
		auto bc2 = static_cast<BoxCollider2D*>(c2);
		overlaping = IsCircleBoxOverlap(cc1, bc2);
	}

	return overlaping;
}

bool PhysicsSystem2D::IsCircleCircleOverlap(CircleCollider2D* c1, CircleCollider2D* c2)
{
	auto t1 = c1->entity->GetComponent<Transform2D>();
	auto t2 = c2->entity->GetComponent<Transform2D>();

	auto c1_c2 = t1->GetWorldPosition() - t2->GetWorldPosition();
	float disSqr = glm::length2(c1_c2);
	float r1r2 = c1->radius + c2->radius;

	if (disSqr >= (r1r2 * r1r2)) return false;

	return true;
}

bool PhysicsSystem2D::IsBoxBoxOverlap(BoxCollider2D* b1, BoxCollider2D* b2)
{
	auto t1 = b1->entity->GetComponent<Transform2D>();
	auto t2 = b2->entity->GetComponent<Transform2D>();

	auto c1 = t1->GetWorldPosition();
	auto c2 = t2->GetWorldPosition();

	auto deltaCenters = c1 - c2;

	auto sumWidths = b1->halfWidth + b2->halfWidth;

	if (glm::abs(deltaCenters.x) > sumWidths) return false;

	auto sumHeights = b1->halfHeight + b2->halfHeight;

	if (glm::abs(deltaCenters.y) > sumHeights) return false;

	return true;
}

bool PhysicsSystem2D::IsCircleBoxOverlap(CircleCollider2D* c, BoxCollider2D* b)
{
	auto tc = c->entity->GetComponent<Transform2D>();
	auto tb = b->entity->GetComponent<Transform2D>();

	//glm::vec2 boxClosestPointToCircle = glm::clamp(tb->GetWorldPosition() + , )
	return false;
}

void Collider2D::AddToPhysicsSystem(PhysicsSystem2D* physicsSystem)
{
	physicsSystem->colliders2D.push_back(this);
}

void Physics::UpdatePhysics(float dt)
{	
	auto t = entity->GetComponent<Transform2D>();
	auto p = t->GetWorldPosition();

	glm::vec2 dv = f * dt;
	v += dv;
	glm::vec2 dp = v * dt;
	p += dp;

	t->SetWorldPosition(p);
	f = { 0,0 };
}

void Gravity::Update(float dt)
{
	auto physics = entity->GetComponent<Physics>();
	if (!physics) return;

	physics->f += force;
}

void CollisionResolutionBounce::Update(float dt)
{
}

void CollisionResolutionBlock::Resolve(Collider2D* other)
{

}

void CollisionResolutionBlock::ResolveCircleCircle(CircleCollider2D* other)
{

}
