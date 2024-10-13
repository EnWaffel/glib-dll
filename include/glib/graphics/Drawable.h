#pragma once

#include "../DLLDefs.h"

namespace glib
{
	class Drawable
	{
	public:
		bool visible;
	public:
		GLIB_API virtual void Draw() = 0;
		GLIB_API virtual void Update(float delta) = 0;
	};
}