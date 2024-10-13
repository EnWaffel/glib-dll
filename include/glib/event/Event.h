#pragma once

#include "../DLLDefs.h"
#include "../utils/KeyCode.h"
#include "../utils/MouseButton.h"

// Events

#define GLIB_EVENT_KEY_PRESS 0x0001
#define GLIB_EVENT_KEY_RELEASE 0x0002
#define GLIB_EVENT_WINDOW_RESIZE 0x0004
#define GLIB_EVENT_MOUSE_PRESS 0x0008
#define GLIB_EVENT_MOUSE_RELEASE 0x0010
#define GLIB_EVENT_MOUSE_MOVE 0x0020

#define GLIB_EVENT_ALL GLIB_EVENT_KEY_PRESS | GLIB_EVENT_KEY_RELEASE | GLIB_EVENT_WINDOW_RESIZE

namespace glib
{
	struct GLIB_API Event
	{
		int id;
	};

	struct GLIB_API KeyEvent : Event
	{
		KeyCode key;
	};

	struct GLIB_API WindowResizeEvent : Event
	{
		int x;
		int y;
		int width;
		int height;
	};

	struct GLIB_API MouseEvent : Event
	{
		MouseButton btn;
	};

	struct GLIB_API MouseMoveEvent : Event
	{
		float x;
		float y;
	};
}