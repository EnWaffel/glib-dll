#include "glib/math/MathFunctions.h"

#include <glm.hpp>

float glib::Lerp(float a, float b, float f)
{
    return a + f * (b - a);
}

float glib::Lerp(const Vec2& vec, float f)
{
    return vec.x + f * (vec.y - vec.x);
}

bool glib::LineRectIntersection(float rP1, float rP2, float rS1, float rS2, float lp1, float lp1_1, float lp2, float lp2_1)
{
    return ((lp1 >= rP1 && lp1 <= (rP1 + rS1)) && (lp1_1 >= rP2 && lp1_1 <= (rP2 + rS2))) || ((lp2 >= rP1 && lp2 <= (rP1 + rS1)) && (lp2_1 >= rP2 && lp2_1 <= (rP2 + rS2)));
}

bool glib::RectRectIntersection(const Vec2& rect1Pos, const Vec2& rect1Size, const Vec2& rect2Pos, const Vec2& rect2Size)
{
    float tw = rect1Size.x;
    float th = rect1Size.y;
    float rw = rect2Size.x;
    float rh = rect2Size.y;
    if (rw <= 0 || rh <= 0 || tw <= 0 || th <= 0) {
        return false;
    }
    float tx = rect1Pos.x;
    float ty = rect1Pos.y;
    float rx = rect2Pos.x;
    float ry = rect2Pos.y;
    rw += rx;
    rh += ry;
    tw += tx;
    th += ty;
    return ((rw < rx || rw > tx) &&
        (rh < ry || rh > ty) &&
        (tw < tx || tw > rx) &&
        (th < ty || th > ry));
}

float glib::ToRadians(float angle)
{
    return glm::radians(angle);
}
