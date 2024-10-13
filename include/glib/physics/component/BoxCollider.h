#pragma once

#include "PhysicsComponent.h"
#include "../../math/Vec2.h"
#include "../../utils/Face.h"

namespace glib
{
	class PhysicsObject;

	class BoxCollider : public PhysicsComponent
	{
	public:
		Vec2 p1;
		Vec2 p2;
		PhysicsObject* obj;
	public:
		GLIB_API BoxCollider(PhysicsObject* obj, const Vec2& p1, const Vec2& p2);
		GLIB_API ~BoxCollider();

		GLIB_API bool CheckFace(Face face, BoxCollider* other);
	};
}