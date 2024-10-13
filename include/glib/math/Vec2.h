#pragma once

#include "../DLLDefs.h"

#define GLIB_VEC2_ZERO glib::Vec2(0.0f, 0.0f)

namespace glib
{
	class GLIB_API Vec2
	{
	public:
		float x;
		float y;
	public:
		Vec2();
		Vec2(float x, float y);

		Vec2 operator+(const Vec2& other);
		Vec2 operator+(float v);
		Vec2 operator-(const Vec2& other);
		Vec2 operator-(float v);
		Vec2 operator*(const Vec2& other);
		Vec2 operator*(float v);
		Vec2 operator/(const Vec2& other);
		Vec2 operator/(float v);
		Vec2 operator^(const Vec2& other);
		Vec2 operator^(float v);

		void operator+=(const Vec2& other);
		void operator+=(float v);
		void operator-=(const Vec2& other);
		void operator-=(float v);
		void operator*=(const Vec2& other);
		void operator*=(float v);
		void operator/=(const Vec2& other);
		void operator/=(float v);
		void operator^=(const Vec2& other);
		void operator^=(float v);

		Vec2 Normalize();

		float Distance(const Vec2& other);
	};
}