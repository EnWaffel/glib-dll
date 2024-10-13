#pragma once

#include "../../DLLDefs.h"

namespace glib
{
	enum GLIB_API PhysicsComponentType
	{
		BOX_COLLIDER,
		RIGIDBODY
	};

	class PhysicsComponent
	{
	public:
		PhysicsComponentType type;
	};
}