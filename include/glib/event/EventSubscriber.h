#pragma once

#include "../DLLDefs.h"
#include "Event.h"

namespace glib
{

	/**
	* A class that can be extended to subscribe to any number of events.
	* Only override the methods that you subscribe to.
	*/
	class EventSubscriber
	{
	public:
		GLIB_API virtual void OnKeyPress(const KeyEvent& event);
		GLIB_API virtual void OnKeyRelease(const KeyEvent& event);
		GLIB_API virtual void OnWindowResize(const WindowResizeEvent& event);
		GLIB_API virtual void OnMouseButtonPress(const MouseEvent& event);
		GLIB_API virtual void OnMouseButtonRelease(const MouseEvent& event);
		GLIB_API virtual void OnMouseMove(const MouseMoveEvent& event);
	};
}