#pragma once

#include "../../DLLDefs.h"
#include "../Drawable.h"
#include "../../math/Mat4.h"
#include "../../utils/Easing.h"
#include "../../math/Vec2.h"

#include <vector>

namespace glib
{
	class CameraImpl;

	class Camera
	{
	public:
		Vec2 pos;
		float zoom;
		float rotation;
	private:
		CameraImpl* impl;
	public:
		GLIB_API Camera(Vec2 initialSize);
		GLIB_API ~Camera();

		GLIB_API void Add(Drawable* drawable);
		GLIB_API void Add(Drawable& drawable);
		GLIB_API void Remove(Drawable* drawable);
		GLIB_API void Remove(Drawable& drawable);
		GLIB_API void RemoveAll();
		GLIB_API std::vector<Drawable*>& GetDrawables();
		GLIB_API Mat4 CalculateView();

		void Update(float delta);

		GLIB_API void TweenPosition(const Vec2& to, float time, const Easing& easing);
		GLIB_API void TweenZoom(float to, float time, const Easing& easing);
		GLIB_API void TweenRotation(float to, float time, const Easing& easing);
	};
}