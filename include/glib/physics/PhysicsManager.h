#pragma once

#include "../DLLDefs.h"
#include "PhysicsObject.h"

namespace glib
{
	class PhysicsManagerImpl;

	class PhysicsManager
	{
	private:
		PhysicsManagerImpl* impl;
	public:
		Vec2 gravity = Vec2(0.0f, -9.81f * 8.0f);
	public:
		GLIB_API PhysicsManager();
		GLIB_API ~PhysicsManager();

		GLIB_API void Add(PhysicsObject& obj);
		GLIB_API void Add(PhysicsObject* obj);
		GLIB_API void Remove(PhysicsObject& obj);
		GLIB_API void Remove(PhysicsObject* obj);

		GLIB_API void Update(float delta);
	};
}