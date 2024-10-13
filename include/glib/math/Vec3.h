#pragma once

#include "../DLLDefs.h"

namespace glib
{
	class GLIB_API Vec3
	{
	public:
		float x;
		float y;
		float z;
	public:
		Vec3();
		Vec3(float x, float y, float z);

		Vec3 operator+(const Vec3& other);
		Vec3 operator+(float v);
		Vec3 operator-(const Vec3& other);
		Vec3 operator-(float v);
		Vec3 operator*(const Vec3& other);
		Vec3 operator*(float v);
		Vec3 operator/(const Vec3& other);
		Vec3 operator/(float v);
		Vec3 operator^(const Vec3& other);
		Vec3 operator^(float v);

		void operator+=(const Vec3& other);
		void operator+=(float v);
		void operator-=(const Vec3& other);
		void operator-=(float v);
		void operator*=(const Vec3& other);
		void operator*=(float v);
		void operator/=(const Vec3& other);
		void operator/=(float v);
		void operator^=(const Vec3& other);
		void operator^=(float v);

		Vec3 operator-();

		Vec3 Normalize();
		Vec3 Cross(const Vec3& other);

		float Distance(const Vec3& other);
	};
}