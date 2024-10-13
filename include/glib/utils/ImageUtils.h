#pragma once

#include "../DLLDefs.h"
#include "../graphics/Texture.h"

namespace glib
{
	class ImageUtils
	{
	public:
		ImageUtils() = delete;
		~ImageUtils() = delete;

		GLIB_API static ImageData Cut(const ImageData& data, int x, int y, int width, int height);
		GLIB_API static void FreeData(const ImageData& data);
	};
}