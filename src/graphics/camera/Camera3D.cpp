#include "glib/graphics/camera/Camera3D.h"
#include "glib/math/MathFunctions.h"
#include "glib/window/Window.h"
#include "glib/event/EventManager.h"

#include <vector>
#include <tweeny.h>

extern tweeny::easing::enumerated __translate_easing(glib::Easing easing);

namespace glib
{
	class Camera3DImpl : public EventSubscriber
	{
	private:
		Camera3D* m_Camera;
		std::vector<Drawable*> m_Drawables;
		tweeny::tween<float, float, float> m_PosTween;
		bool m_PosTweenActive = false;
		tweeny::tween<float, float, float> m_RotTween;
		bool m_RotTweenActive = false;
		tweeny::tween<float> m_PitchTween;
		bool m_PitchTweenActive = false;
		tweeny::tween<float> m_YawTween;
		bool m_YawTweenActive = false;

		float m_PosTweenDelay = 0.0f;
		float m_RotTweenDelay = 0.0f;
		float m_PitchTweenDelay = 0.0f;
		float m_YawTweenDelay = 0.0f;

		Vec2 m_InitialSize;
		bool m_Freelook = false;
		Vec2 m_FreelookPitch = Vec2(-90.0f, 90.0f);
		Vec2 m_FreelookYaw;
		bool m_FirstMouseMove = true;
		float m_LastX = 0.0f;
		float m_LastY = 0.0f;
		float m_FreelookSpeed = 0.1f;
		Window* m_Wnd;
	public:
		Camera3DImpl(Camera3D* camera, Vec2 initialSize, Window* wnd) : m_Camera(camera), m_InitialSize(initialSize), m_Wnd(wnd)
		{
			wnd->GetEventManager().Subscribe(GLIB_EVENT_MOUSE_MOVE, this);
		}

		~Camera3DImpl()
		{
			m_Wnd->GetEventManager().Unsubscribe(this);
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
				if (m_PosTweenDelay > 0.0f)
				{
					m_PosTweenDelay -= delta;
				}
				else
				{
					if (m_PosTween.progress() >= 1.0f)
					{
						m_PosTweenActive = false;
					}
					m_PosTween.step((int)delta);
				}
			}
			if (m_RotTweenActive)
			{
				if (m_RotTweenDelay > 0.0f)
				{
					m_RotTweenDelay -= delta;
				}
				else
				{
					if (m_RotTween.progress() >= 1.0f)
					{
						m_RotTweenActive = false;
					}
					m_RotTween.step((int)delta);
				}
			}
			if (m_PitchTweenActive)
			{
				if (m_PitchTweenDelay > 0.0f)
				{
					m_PitchTweenDelay -= delta;
				}
				else
				{
					if (m_PitchTween.progress() >= 1.0f)
					{
						m_PitchTweenActive = false;
					}
					m_PitchTween.step((int)delta);
				}
			}
			if (m_YawTweenActive)
			{
				if (m_YawTweenDelay > 0.0f)
				{
					m_YawTweenDelay -= delta;
				}
				else
				{
					if (m_YawTween.progress() >= 1.0f)
					{
						m_YawTweenActive = false;
					}
					m_YawTween.step((int)delta);
				}
			}
		}

		void TweenPosition(const Vec3& to, float time, const Easing& easing, float delay)
		{
			m_PosTweenActive = true;
			m_PosTweenDelay = delay;
			m_PosTween = tweeny::from(m_Camera->pos.x, m_Camera->pos.y, m_Camera->pos.z).to(to.x, to.y, to.z).during(time).via(__translate_easing(easing)).onStep([this](float x, float y, float z) {
				m_Camera->pos.x = x;
				m_Camera->pos.y = y;
				m_Camera->pos.z = z;
				return false;
			});
		}

		void TweenRotation(const Vec3& to, float time, const Easing& easing, float delay)
		{
			m_RotTweenActive = true;
			m_RotTweenDelay = delay;
			m_RotTween = tweeny::from(m_Camera->rotation.x, m_Camera->rotation.y, m_Camera->rotation.z).to(to.x, to.y, to.z).during(time).via(__translate_easing(easing)).onStep([this](float x, float y, float z) {
				m_Camera->rotation.x = x;
				m_Camera->rotation.y = y;
				m_Camera->rotation.z = z;
				return false;
			});
		}

		void TweenPitch(float to, float time, const Easing& easing, float delay)
		{
			m_PitchTweenActive = true;
			m_PitchTweenDelay = delay;
			m_PitchTween = tweeny::from(m_Camera->pitch).to(to).during(time).via(__translate_easing(easing)).onStep([this](float pitch) {
				m_Camera->pitch = pitch;
				return false;
			});
		}

		void TweenYaw(float to, float time, const Easing& easing, float delay)
		{
			m_YawTweenActive = true;
			m_YawTweenDelay = delay;
			m_YawTween = tweeny::from(m_Camera->yaw).to(to).during(time).via(__translate_easing(easing)).onStep([this](float yaw) {
				m_Camera->yaw = yaw;
				return false;
				});
		}

		void SetFreelook(bool enabled)
		{
			m_Freelook = enabled;
			m_FirstMouseMove = true;
		}

		void SetFreelookConstraints(const Vec2& pitchMinMax, const Vec2& yawMinMax)
		{
			m_FreelookPitch = pitchMinMax;
			m_FreelookYaw = yawMinMax;
		}

		void OnMouseMove(const MouseMoveEvent& event) override
		{
			if (!m_Freelook) return;

			if (m_FirstMouseMove)
			{
				m_LastX = event.x;
				m_LastY = event.y;
				m_FirstMouseMove = false;
			}

			float offsetX = event.x - m_LastX;
			float offsetY = m_LastY - event.y;
			offsetX *= m_FreelookSpeed;
			offsetY *= m_FreelookSpeed;

			m_LastX = event.x;
			m_LastY = event.y;

			m_Camera->yaw += offsetX;
			m_Camera->pitch += offsetY;

			if (m_FreelookPitch.x != 0.0f)
			{
				if (m_Camera->pitch >= m_FreelookPitch.y) m_Camera->pitch = m_FreelookPitch.y;
				if (m_Camera->pitch <= m_FreelookPitch.x) m_Camera->pitch = m_FreelookPitch.x;
			}
			if (m_FreelookYaw.x != 0.0f)
			{
				if (m_Camera->yaw >= m_FreelookYaw.y) m_Camera->yaw = m_FreelookYaw.y;
				if (m_Camera->yaw <= m_FreelookYaw.x) m_Camera->yaw = m_FreelookYaw.x;
			}
		}

		void SetFreelookSpeed(float speed)
		{
			m_FreelookSpeed = speed;
		}

		Mat4 CalculateView()
		{
			Vec3 _front;
			_front.x = cos(ToRadians(m_Camera->yaw)) * cos(ToRadians(m_Camera->pitch));
			_front.y = sin(ToRadians(m_Camera->pitch));
			_front.z = sin(ToRadians(m_Camera->yaw)) * cos(ToRadians(m_Camera->pitch));
			m_Camera->front = _front.Normalize();

			m_Camera->right = m_Camera->front.Cross(m_Camera->worldUp).Normalize();
			m_Camera->up = m_Camera->right.Cross(m_Camera->front).Normalize();

			Mat4 mat = Mat4::LookAt(m_Camera->pos, m_Camera->pos + m_Camera->front, m_Camera->up);

			mat.Rotate(m_Camera->rotation.x, Vec3(1.0f, 0.0f, 0.0f));
			mat.Rotate(m_Camera->rotation.y, Vec3(0.0f, 1.0f, 0.0f));
			mat.Rotate(m_Camera->rotation.z, Vec3(0.0f, 0.0f, 1.0f));

			return mat;
		}
	};
}

using namespace glib;

glib::Camera3D::Camera3D(Vec2 initialSize, Window* wnd) : pos(Vec3(0.0f, 0.0f, 0.0f)), yaw(90.0f), pitch(0.0f)
{
	impl = new Camera3DImpl(this, initialSize, wnd);

	up = glib::Vec3(0.0f, 1.0f, 0.0f);
	worldUp = up;
	front = glib::Vec3(0.0f, 0.0f, -1.0f);
}

glib::Camera3D::~Camera3D()
{
	delete impl;
}

void glib::Camera3D::Add(Drawable* drawable)
{
	impl->Add(drawable);
}

void glib::Camera3D::Add(Drawable& drawable)
{
	impl->Add(drawable);
}

void glib::Camera3D::Remove(Drawable* drawable)
{
	impl->Remove(drawable);
}

void glib::Camera3D::Remove(Drawable& drawable)
{
	impl->Remove(&drawable);
}

void glib::Camera3D::RemoveAll()
{
	impl->RemoveAll();
}

std::vector<Drawable*>& glib::Camera3D::GetDrawables()
{
	return impl->GetDrawables();
}

Mat4 glib::Camera3D::CalculateView()
{
	return impl->CalculateView();
}

void glib::Camera3D::SetFreelook(bool enabled)
{
	impl->SetFreelook(enabled);
}

void glib::Camera3D::SetFreelookConstraints(const Vec2& pitchMinMax, const Vec2& yawMinMax)
{
	impl->SetFreelookConstraints(pitchMinMax, yawMinMax);
}

void glib::Camera3D::SetFreelookSpeed(float speed)
{
	impl->SetFreelookSpeed(speed);
}

void glib::Camera3D::Update(float delta)
{
	impl->Update(delta);
}

void glib::Camera3D::TweenPosition(const Vec3& to, float time, const Easing& easing, float delay)
{
	impl->TweenPosition(to, time, easing, delay);
}

void glib::Camera3D::TweenRotation(const Vec3& to, float time, const Easing& easing, float delay)
{
	impl->TweenRotation(to, time, easing, delay);
}

void glib::Camera3D::TweenPitch(float to, float time, const Easing& easing, float delay)
{
	impl->TweenPitch(to, time, easing, delay);
}

void glib::Camera3D::TweenYaw(float to, float time, const Easing& easing, float delay)
{
	impl->TweenYaw(to, time, easing, delay);
}
