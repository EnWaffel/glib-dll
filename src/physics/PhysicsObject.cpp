#include "glib/physics/PhysicsObject.h"

#include "glib/physics/component/BoxCollider.h"

using namespace glib;

static Vec2 GetBoxColliderCenter(PhysicsObject* obj, BoxCollider* c)
{
	return Vec2(
		(obj->pos.x + c->p1.x) - obj->pos.x + c->p2.x / 2.0f,
		(obj->pos.y + c->p1.y) - obj->pos.y + c->p2.y / 2.0f
	);
}

float glib::PhysicsObject::Distance(PhysicsObject* other)
{
	PhysicsComponent* c = nullptr;
	PhysicsComponent* c1 = nullptr;
	c = GetFirstComponent(BOX_COLLIDER);
	if (c == nullptr)
	{
		return 0.0f;
	}

	c1 = GetFirstComponent(BOX_COLLIDER);
	if (c1 == nullptr)
	{
		return 0.0f;
	}

	Vec2 p;
	Vec2 p1;

	if (c->type == BOX_COLLIDER)
	{
		BoxCollider* bc = (BoxCollider*)c;
		p = GetBoxColliderCenter(this, bc);
	}

	if (c1->type == BOX_COLLIDER)
	{
		BoxCollider* bc = (BoxCollider*)c1;
		p1 = GetBoxColliderCenter(other, bc);
	}

	return p.Distance(p1);
}

PhysicsComponent* glib::PhysicsObject::GetFirstComponent(PhysicsComponentType type)
{
	for (PhysicsComponent* c : components)
	{
		if (c->type == type)
		{
			return c;
		}
	}
	return nullptr;
}

bool glib::PhysicsObject::HasComponent(PhysicsComponentType type)
{
	return GetFirstComponent(type) != nullptr;
}
