#include "glib/math/Vec2i.h"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace glib;

glib::Vec2i::Vec2i() : x(0.0f), y(0.0f)
{
}

glib::Vec2i::Vec2i(int x, int y) : x(x), y(y)
{
}

Vec2i glib::Vec2i::operator+(const Vec2i& other) {
    return Vec2i(x + other.x, y + other.y);
}

Vec2i glib::Vec2i::operator+(int v)
{
    return Vec2i(x + v, y + v);
}

Vec2i glib::Vec2i::operator-(const Vec2i& other)
{
    return Vec2i(x - other.x, y - other.y);
}

Vec2i glib::Vec2i::operator-(int v)
{
    return Vec2i(x - v, y - v);
}

Vec2i glib::Vec2i::operator*(const Vec2i& other)
{
    return Vec2i(x * other.x, y * other.y);
}

Vec2i glib::Vec2i::operator*(int v)
{
    return Vec2i(x * v, y * v);
}

Vec2i glib::Vec2i::operator/(const Vec2i& other)
{
    return Vec2i(x / other.x, y / other.y);
}

Vec2i glib::Vec2i::operator/(int v)
{
    return Vec2i(x / v, y / v);
}

Vec2i glib::Vec2i::operator^(const Vec2i& other)
{
    return Vec2i(powf(x, other.x), powf(y, other.y));
}

Vec2i glib::Vec2i::operator^(int v)
{
    return Vec2i(powf(x, v), powf(y, v));
}

void glib::Vec2i::operator+=(const Vec2i& other)
{
    x += other.x;
    y += other.y;
}

void glib::Vec2i::operator+=(int v)
{
    x += v;
    y += v;
}

void glib::Vec2i::operator-=(const Vec2i& other)
{
    x -= other.x;
    y -= other.y;
}

void glib::Vec2i::operator-=(int v)
{
    x -= v;
    y -= v;
}

void glib::Vec2i::operator*=(const Vec2i& other)
{
    x *= other.x;
    y *= other.y;
}

void glib::Vec2i::operator*=(int v)
{
    x *= v;
    y *= v;
}

void glib::Vec2i::operator/=(const Vec2i& other)
{
    x /= other.x;
    y /= other.y;
}

void glib::Vec2i::operator/=(int v)
{
    x /= v;
    y /= v;
}

void glib::Vec2i::operator^=(const Vec2i& other)
{
    x = powf(x, other.x);
    y = powf(y, other.y);
}

void glib::Vec2i::operator^=(int v)
{
    x = powf(x, v);
    y = powf(y, v);
}

int glib::Vec2i::Distance(const Vec2i& other)
{
    return sqrtf(powf((other.x - x), 2) + powf(other.y - y, 2));
}
