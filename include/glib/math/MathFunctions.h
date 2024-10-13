#pragma once

#include "Vec2.h"
#include "../DLLDefs.h"

namespace glib
{
	GLIB_API float Lerp(float a, float b, float f);
	GLIB_API float Lerp(const Vec2& vec, float f);
	GLIB_API bool LineRectIntersection(float rP1, float rP2, float rS1, float rS2, float lp1, float lp1_1, float lp2, float lp2_1);
	GLIB_API bool RectRectIntersection(const Vec2& rect1Pos, const Vec2& rect1Size, const Vec2& rect2Pos, const Vec2& rect2Size);
	GLIB_API float ToRadians(float angle);
}