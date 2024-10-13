#pragma once

#include "../DLLDefs.h"
#include <string>
#include "glib/math/Mat4.h"
#include "../utils/Color.h"

namespace glib
{
	class ShaderImpl;

	class Shader
	{
	private:
		ShaderImpl* impl;
	public:
		Shader(unsigned int id);
		~Shader();

		GLIB_API void Use() const;
		GLIB_API int GetUniformLocation(const std::string& name);
		GLIB_API void SetMat4(const std::string& name, Mat4& mat);
		GLIB_API void SetInt(const std::string& name, int i);
		GLIB_API void SetColor(const std::string& name, const Color& color);
		GLIB_API void SetFloat(const std::string& name, float f);
		GLIB_API void SetVec2(const std::string& name, const Vec2& vec);
	};
}