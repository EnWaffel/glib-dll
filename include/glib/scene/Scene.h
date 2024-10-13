#pragma once

#include "../DLLDefs.h"
#include "../window/Window.h"

namespace glib
{
	class Scene
	{
	public:
		GLIB_API virtual ~Scene();

		GLIB_API virtual void Enable(Window* wnd) = 0;
		GLIB_API virtual void Update(float delta, Window* wnd) = 0;
		GLIB_API virtual void Disable(Window* wnd) = 0;
	};
}