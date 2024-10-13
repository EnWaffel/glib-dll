#include "glib/math/Mat4.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

namespace glib
{
	class Mat4Impl
	{
	public:
		glm::mat4 mat;
	public:
		Mat4Impl()
		{
			mat = glm::mat4(1.0f);
		}

		Mat4Impl(const glm::mat4& m)
		{
			mat = m;
		}

		void Translate(const Vec2& vec)
		{
			mat = glm::translate(mat, glm::vec3(vec.x, vec.y, 0.0f));
		}

		void Scale(const Vec2& vec)
		{
			mat = glm::scale(mat, glm::vec3(vec.x, vec.y, 0.0f));
		}

		void Translate(const Vec3& vec)
		{
			mat = glm::translate(mat, glm::vec3(vec.x, vec.y, vec.z));
		}

		void Scale(const Vec3& vec)
		{
			mat = glm::scale(mat, glm::vec3(vec.x, vec.y, vec.z));
		}

		void Rotate(float deg, const Vec3& axis)
		{
			mat = glm::rotate(mat, glm::radians(deg), glm::vec3(axis.x, axis.y, axis.z));
		}

		void* GetPtr()
		{
			return &mat;
		}

		Mat4 Mult(Mat4& other)
		{
			Mat4 m;
			m.impl->mat = mat * other.impl->mat;
			return m;
		}
	};
}

using namespace glib;

glib::Mat4::Mat4()
{
	impl = new Mat4Impl;
}

glib::Mat4::~Mat4()
{
	delete impl;
}

void glib::Mat4::Translate(const Vec3& vec)
{
	impl->Translate(vec);
}

void glib::Mat4::Scale(const Vec3& vec)
{
	impl->Scale(vec);
}

void glib::Mat4::Translate(const Vec2& vec)
{
	impl->Translate(vec);
}

void glib::Mat4::Scale(const Vec2& vec)
{
	impl->Scale(vec);
}

void glib::Mat4::Rotate(float deg, const Vec3& axis)
{
	impl->Rotate(deg, axis);
}

Mat4 glib::Mat4::operator*(Mat4& other)
{
	return impl->Mult(other);
}

void* glib::Mat4::GetPtr()
{
	return impl->GetPtr();
}

Mat4 glib::Mat4::Copy()
{
	Mat4 m;
	m.impl->mat = impl->mat;
	return m;
}

Mat4 glib::Mat4::Ortho(float left, float right, float bottom, float top)
{
	Mat4 m;
	m.impl->mat = glm::ortho(left, right, bottom, top);
	return m;
}

Mat4 glib::Mat4::Perspective(float fov, float aspect, float near, float far)
{
	Mat4 m;
	m.impl->mat = glm::perspective(fov, aspect, near, far);
	return m;
}

Mat4 glib::Mat4::LookAt(const Vec3& eye, const Vec3& center, const Vec3& up)
{
	Mat4 m;
	m.impl->mat = glm::lookAt(glm::vec3(eye.x, eye.y, eye.z), glm::vec3(center.x, center.y, center.z), glm::vec3(up.x, up.y, up.z));
	return m;
}
