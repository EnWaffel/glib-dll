#pragma once

#include "../DLLDefs.h"
#include "Vec2.h"
#include "Vec3.h"

namespace glib
{
	class Mat4Impl;

	class Mat4
	{
	private:
		Mat4Impl* impl;
	public:
		GLIB_API Mat4();
		GLIB_API ~Mat4();

		GLIB_API void Translate(const Vec2& vec);
		GLIB_API void Scale(const Vec2& vec);
		GLIB_API void Translate(const Vec3& vec);
		GLIB_API void Scale(const Vec3& vec);
		GLIB_API void Rotate(float deg, const Vec3& axis = Vec3(0.0f, 0.0f, 1.0f)); // Z-Axis for 2D

		GLIB_API Mat4 operator*(Mat4& other);
			
		GLIB_API void* GetPtr();
		GLIB_API Mat4 Copy();

		friend class Mat4Impl;
	public:
		GLIB_API static Mat4 Ortho(float left, float right, float bottom, float top);
		GLIB_API static Mat4 Perspective(float fov, float aspect, float near, float far);
		GLIB_API static Mat4 LookAt(const Vec3& eye, const Vec3& center, const Vec3& up);
	};
}