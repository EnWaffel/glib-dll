#include "glib/physics/component/BoxCollider.h"
#include "glib/math/MathFunctions.h"
#include "glib/physics/PhysicsObject.h"

using namespace glib;

glib::BoxCollider::BoxCollider(PhysicsObject* obj, const Vec2& p1, const Vec2& p2) : obj(obj), p1(p1), p2(p2)
{
}

glib::BoxCollider::~BoxCollider()
{
}

bool glib::BoxCollider::CheckFace(Face face, BoxCollider* other)
{
	Vec2 pos(obj->pos.x + p1.x, obj->pos.y + p1.y);
	Vec2 size(p1.x + p2.x, p1.y + p2.y);

	Vec2 pos1(other->obj->pos.x + other->p1.x, other->obj->pos.y + other->p1.y);
	Vec2 size1(other->p1.x + other->p2.x, other->p1.y + other->p2.y);

	switch (face)
	{
	case TOP:
	{
		return LineRectIntersection
		(
			pos1.x,
			pos1.y,
			size1.x,
			size1.y,
			pos.x,
			pos.y - 1,
			pos.x + size.x,
			pos.y - 1
		);
	}
	case BOTTOM:
	{
		return LineRectIntersection
		(
			pos1.x,
			pos1.y,
			size1.x,
			size1.y,
			pos.x,
			pos.y + size.y + 1,
			pos.x + size.x,
			pos.y + size.y + 1
		);
	}
	case LEFT:
	{
		break;
	}
	case RIGHT:
	{
		break;
	}
	}
    return false;
}
