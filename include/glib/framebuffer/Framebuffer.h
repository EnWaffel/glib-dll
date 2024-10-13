#pragma once

#include "../DLLDefs.h"
#include "../math/Vec2.h"

namespace glib
{

	enum class GLIB_API FramebufferType
	{
		TEXTURE,
		//RENDERBUFFER
	};

	struct FBInternals
	{
		unsigned int tex;
		//unsigned int RBO;
	};

	class FramebufferImpl;

	/**
	* A convenience wrapper of an FBO.
	*/
	class Framebuffer
	{
	private:
		FramebufferImpl* impl;
	public:
		GLIB_API Framebuffer(FramebufferType type, const Vec2& dimensions);
		GLIB_API ~Framebuffer();

		GLIB_API void Bind();
		GLIB_API void Unbind();

		GLIB_API FBInternals GetInternals();
	};
}