#include "glib/math/Rect.h"

using namespace glib;

glib::Rect::Rect() : x(0.0f), y(0.0f), w(0.0f), h(0.0f)
{
}

glib::Rect::Rect(float x, float y, float w, float h) : x(x), y(y), w(w), h(h)
{
}

float glib::Rect::Distance(const Rect& other)
{
	return 0.0f;
}
