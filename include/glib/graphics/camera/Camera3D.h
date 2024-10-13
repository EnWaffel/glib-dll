#pragma once

#include "../../DLLDefs.h"
#include "../Drawable.h"
#include "../../math/Mat4.h"
#include "../../utils/Easing.h"
#include "../../math/Vec2.h"
#include "../../math/Vec3.h"
#include "../../window/Window.h"

#include <vector>

namespace glib
{
	class Camera3DImpl;

	class Camera3D
	{
	public:
		Vec3 pos;
		Vec3 front;
		Vec3 up;
		Vec3 right;
		Vec3 worldUp;
		Vec3 rotation;
		float yaw;
		float pitch;
		float fov;
	private:
		Camera3DImpl* impl;
	public:
		GLIB_API Camera3D(Vec2 initialSize, Window* wnd);
		GLIB_API ~Camera3D();

		GLIB_API void Add(Drawable* drawable);
		GLIB_API void Add(Drawable& drawable);
		GLIB_API void Remove(Drawable* drawable);
		GLIB_API void Remove(Drawable& drawable);
		GLIB_API void RemoveAll();
		GLIB_API std::vector<Drawable*>& GetDrawables();
		GLIB_API Mat4 CalculateView();
		GLIB_API void SetFreelook(bool enabled);
		GLIB_API void SetFreelookConstraints(const Vec2& pitchMinMax, const Vec2& yawMinMax);
		GLIB_API void SetFreelookSpeed(float speed);

		void Update(float delta);

		GLIB_API void TweenPosition(const Vec3& to, float time, const Easing& easing, float delay = 0.0f);
		GLIB_API void TweenRotation(const Vec3& to, float time, const Easing& easing, float delay = 0.0f);
		GLIB_API void TweenPitch(float to, float time, const Easing& easing, float delay = 0.0f);
		GLIB_API void TweenYaw(float to, float time, const Easing& easing, float delay = 0.0f);
	};
}