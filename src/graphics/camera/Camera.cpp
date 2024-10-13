#include "glib/graphics/camera/Camera.h"

#include <vector>
#include <tweeny.h>
#include <iostream>

extern tweeny::easing::enumerated __translate_easing(glib::Easing easing);

namespace glib
{
	class CameraImpl
	{
	private:
		Camera* m_Camera;
		std::vector<Drawable*> m_Drawables;
		tweeny::tween<float, float> m_PosTween;
		bool m_PosTweenActive = false;
		tweeny::tween<float> m_ZoomTween;
		bool m_ZoomTweenActive = false;
		tweeny::tween<float> m_RotTween;
		bool m_RotTweenActive = false;
		Vec2 m_InitialSize;
	public:
		CameraImpl(Camera* camera, Vec2 initialSize) : m_Camera(camera), m_InitialSize(initialSize)
		{
		}

		~CameraImpl()
		{
		}

		void Add(Drawable* drawable)
		{
			m_Drawables.push_back(drawable);
		}

		void Add(Drawable& drawable)
		{
			m_Drawables.push_back(&drawable);
		}

		void Remove(Drawable* drawable)
		{
			std::vector<Drawable*>::iterator it = std::find(m_Drawables.begin(), m_Drawables.end(), drawable);
			if (it == m_Drawables.end()) return;
			m_Drawables.erase(it);
		}

		std::vector<Drawable*>& GetDrawables()
		{
			return m_Drawables;
		}

		void RemoveAll()
		{
			m_Drawables.clear();
		}

		void Update(float delta)
		{
			for (Drawable* drawable : m_Drawables)
			{
				drawable->Update(delta);
			}

			if (m_PosTweenActive)
			{
				if (m_PosTween.progress() >= 1.0f)
				{
					m_PosTweenActive = false;
				}
				m_PosTween.step((int)delta);
			}
			if (m_ZoomTweenActive)
			{
				if (m_ZoomTween.progress() >= 1.0f)
				{
					m_ZoomTweenActive = false;
				}
				m_ZoomTween.step((int)delta);
			}
			if (m_RotTweenActive)
			{
				if (m_RotTween.progress() >= 1.0f)
				{
					m_RotTweenActive = false;
				}
				m_RotTween.step((int)delta);
			}
		}

		void TweenPosition(const Vec2& to, float time, const Easing& easing)
		{
			m_PosTweenActive = true;
			m_PosTween = tweeny::from(m_Camera->pos.x, m_Camera->pos.y).to(to.x, to.y).during(time).via(__translate_easing(easing)).onStep([this](float x, float y) {
				m_Camera->pos.x = x;
				m_Camera->pos.y = y;
				return false;
			});
		}

		void TweenZoom(float to, float time, const Easing& easing)
		{
			m_ZoomTweenActive = true;
			m_ZoomTween = tweeny::from(m_Camera->zoom).to(to).during(time).via(__translate_easing(easing)).onStep([this](float _zoom) {
				m_Camera->zoom = _zoom;
				return false;
			});
		}

		void TweenRotation(float to, float time, const Easing& easing)
		{
			m_RotTweenActive = true;
			m_RotTween = tweeny::from(m_Camera->rotation).to(to).during(time).via(__translate_easing(easing)).onStep([this](float rot) {
				m_Camera->rotation = rot;
				return false;
			});
		}

		Mat4 CalculateView()
		{
			Mat4 m;
			Vec2 diff = Vec2((m_InitialSize.x * m_Camera->zoom) - m_InitialSize.x, (m_InitialSize.y * m_Camera->zoom) - m_InitialSize.y);

			m.Translate(Vec2(m_InitialSize.x / 2.0f * m_Camera->zoom, m_InitialSize.y / 2.0f * m_Camera->zoom));
			m.Rotate(m_Camera->rotation);
			m.Translate(Vec2(-(m_InitialSize.x / 2.0f) * m_Camera->zoom, -(m_InitialSize.y / 2.0f) * m_Camera->zoom));

			m.Translate(Vec2(-diff.x / 2.0f, -diff.y / 2.0f));
			m.Scale(Vec2(m_Camera->zoom, m_Camera->zoom));

			return m;
		}
	};
}

using namespace glib;

glib::Camera::Camera(Vec2 initialSize) : pos(Vec2(0.0f, 0.0f)), zoom(1.0f), rotation(0.0f)
{
	impl = new CameraImpl(this, initialSize);
}

glib::Camera::~Camera()
{
	delete impl;
}

void glib::Camera::Add(Drawable* drawable)
{
	impl->Add(drawable);
}

void glib::Camera::Add(Drawable& drawable)
{
	impl->Add(drawable);
}

void glib::Camera::Remove(Drawable* drawable)
{
	impl->Remove(drawable);
}

void glib::Camera::Remove(Drawable& drawable)
{
	impl->Remove(&drawable);
}

void glib::Camera::RemoveAll()
{
	impl->RemoveAll();
}

std::vector<Drawable*>& glib::Camera::GetDrawables()
{
	return impl->GetDrawables();
}

Mat4 glib::Camera::CalculateView()
{
	return impl->CalculateView();
}

void glib::Camera::Update(float delta)
{
	impl->Update(delta);
}

void glib::Camera::TweenPosition(const Vec2& to, float time, const Easing& easing)
{
	impl->TweenPosition(to, time, easing);
}

void glib::Camera::TweenZoom(float to, float time, const Easing& easing)
{
	impl->TweenZoom(to, time, easing);
}

void glib::Camera::TweenRotation(float to, float time, const Easing& easing)
{
	impl->TweenRotation(to, time, easing);
}
