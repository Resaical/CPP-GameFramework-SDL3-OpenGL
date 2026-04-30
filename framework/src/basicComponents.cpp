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

void BoxCollider2D::AddToPhysicsSystem(PhysicsSystem2D* physicsSystem)
{
	physicsSystem->boxColliders2D.push_back(this);
}

bool BoxCollider2D::IsCollidingThisFrame(PhysicsSystem2D* physicsSystem, Collision& collision)
{
	auto e = this->entity;
	auto t = e->GetComponent<Transform2D>();
	auto c = t->GetWorldPosition();

	for (auto b : physicsSystem->boxColliders2D)
	{
		if (b->entity == this->entity) continue;

		auto e1 = b->entity;
		auto t1 = e1->GetComponent<Transform2D>();
		auto c1 = t1->GetWorldPosition();

		auto deltaCenters = c - c1;

		auto sumWidths = this->halfWidth + b->halfWidth;

		if (glm::abs(deltaCenters.x) > sumWidths) continue;

		auto sumHeights = this->halfHeight + b->halfHeight;

		if (glm::abs(deltaCenters.y) > sumHeights) continue;

		collision.entity = e;
		collision.other = e1;

		return true;
	}

	return false;
}


void PhysicsSystem2D::Update(float dt)
{
	for (auto p : physicsEntities)
	{
		p->UpdatePhysics(dt);
	}
}

void Physics::AddToPhysicsSystem(PhysicsSystem2D* physicsSystem)
{
	physicsSystem->physicsEntities.push_back(this);
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
	auto boxCollider2D = entity->GetComponent<BoxCollider2D>();
	if (!boxCollider2D) return;

	auto physics = entity->GetComponent<Physics>();
	if (!physics) return;

	Collision collision;

	if (!boxCollider2D->IsCollidingThisFrame(physicsSystem2D, collision)) return;

	auto v = physics->v;

	auto nvy = - v.y * bounceFactor;
	physics->v = glm::vec2(v.x, nvy);

	auto dp = glm::vec2(0,nvy) * dt;

	auto t = entity->GetComponent<Transform2D>();
	auto p = t->GetWorldPosition();

	p += dp;
	t->SetWorldPosition(p);

}
