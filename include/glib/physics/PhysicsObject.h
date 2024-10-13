#pragma once

#include "../DLLDefs.h"
#include "component/PhysicsComponent.h"
#include "../math/Vec2.h"

#include <vector>

namespace glib
{
	class PhysicsObject
	{
	public:
		std::vector<PhysicsComponent*> components;
		Vec2 pos = Vec2(0.0f, 0.0f);
		Vec2 velocity = Vec2(0.0f, 0.0f);
		float rotation = 0.0f;
		bool hasGravity = true;
	public:
		GLIB_API float Distance(PhysicsObject* other);
		GLIB_API PhysicsComponent* GetFirstComponent(PhysicsComponentType type);
		GLIB_API bool HasComponent(PhysicsComponentType type);
	};
}