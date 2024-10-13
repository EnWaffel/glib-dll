#pragma once

#include "../DLLDefs.h"

namespace glib
{
	class GLIB_API Vec2i
	{
	public:
		int x;
		int y;
	public:
		Vec2i();
		Vec2i(int x, int y);

		Vec2i operator+(const Vec2i& other);
		Vec2i operator+(int v);
		Vec2i operator-(const Vec2i& other);
		Vec2i operator-(int v);
		Vec2i operator*(const Vec2i& other);
		Vec2i operator*(int v);
		Vec2i operator/(const Vec2i& other);
		Vec2i operator/(int v);
		Vec2i operator^(const Vec2i& other);
		Vec2i operator^(int v);

		void operator+=(const Vec2i& other);
		void operator+=(int v);
		void operator-=(const Vec2i& other);
		void operator-=(int v);
		void operator*=(const Vec2i& other);
		void operator*=(int v);
		void operator/=(const Vec2i& other);
		void operator/=(int v);
		void operator^=(const Vec2i& other);
		void operator^=(int v);

		int Distance(const Vec2i& other);
	};
}