#include "glib/graphics/Sprite.h"
#include "glib/utils/Easing.h"
#include "glib/animation/AnimationManager.h"

#include <iostream>
#include <tweeny.h>

extern tweeny::easing::enumerated __translate_easing(glib::Easing easing);

namespace glib
{
	class SpriteImpl : public Animatable
	{
	private:
		Sprite* m_Sprite;
		tweeny::tween<float, float> m_PosTween;
		bool m_PosTweenActive = false;
		tweeny::tween<float, float> m_SizeTween;
		bool m_SizeTweenActive = false;
		tweeny::tween<float, float> m_ScaleTween;
		bool m_ScaleTweenActive = false;
		tweeny::tween<float, float, float, float> m_ColorTween;
		bool m_ColorTweenActive = false;
		tweeny::tween<float> m_RotationTween;
		bool m_RotationTweenActive = false;
		tweeny::tween<float, float> m_OffsetTween;
		bool m_OffsetTweenActive = false;
		
		float m_PosTweenDelay = 0.0f;
		float m_SizeTweenDelay = 0.0f;
		float m_ScaleTweenDelay = 0.0f;
		float m_ColorTweenDelay = 0.0f;
		float m_RotationTweenDelay = 0.0f;
		float m_OffsetTweenDelay = 0.0f;

		AnimationManager m_AnimationManager;
		bool m_AutoAnimCenter;
		Axis m_AutoAnimCenterAxis;
		Vec2 m_AutoAnimCenterContainerSize;
	public:
		SpriteImpl(Sprite* s) : m_Sprite(s), m_AnimationManager(this)
		{
		}

		void Update(float delta)
		{
 			m_AnimationManager.Update(delta);

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
			if (m_SizeTweenActive)
			{
				if (m_SizeTweenDelay > 0.0f)
				{
					m_SizeTweenDelay -= delta;
				}
				else
				{
					if (m_SizeTween.progress() >= 1.0f)
					{
						m_SizeTweenActive = false;
					}
					m_SizeTween.step((int)delta);
				}
			}
			if (m_ScaleTweenActive)
			{
				if (m_ScaleTweenDelay > 0.0f)
				{
					m_ScaleTweenDelay -= delta;
				}
				else
				{
					if (m_ScaleTween.progress() >= 1.0f)
					{
						m_ScaleTweenActive = false;
					}
					m_ScaleTween.step((int)delta);
				}
			}
			if (m_ColorTweenActive)
			{
				if (m_ColorTweenDelay > 0.0f)
				{
					m_ColorTweenDelay -= delta;
				}
				else
				{
					if (m_ColorTween.progress() >= 1.0f)
					{
						m_ColorTweenActive = false;
					}
					m_ColorTween.step((int)delta);
				}
			}
			if (m_RotationTweenActive)
			{
				if (m_RotationTweenDelay > 0.0f)
				{
					m_RotationTweenDelay -= delta;
				}
				else
				{
					if (m_RotationTween.progress() >= 1.0f)
					{
						m_RotationTweenActive = false;
					}
					m_RotationTween.step((int)delta);
				}
			}
			if (m_OffsetTweenActive)
			{
				if (m_OffsetTweenDelay > 0.0f)
				{
					m_OffsetTweenDelay -= delta;
				}
				else
				{
					if (m_OffsetTween.progress() >= 1.0f)
					{
						m_OffsetTweenActive = false;
					}
					m_OffsetTween.step((int)delta);
				}
			}
		}

		void TweenPosition(const Vec2& to, float time, const Easing& easing, float delay)
		{
			m_PosTweenActive = true;
			m_PosTweenDelay = delay;
			m_PosTween = tweeny::from(m_Sprite->pos.x, m_Sprite->pos.y).to(to.x, to.y).during(time).via(__translate_easing(easing)).onStep([this](float x, float y) {
				m_Sprite->pos.x = x;
				m_Sprite->pos.y = y;
				return false;
			});
		}

		void TweenSize(const Vec2& to, float time, const Easing& easing, float delay)
		{
			m_SizeTweenActive = true;
			m_SizeTweenDelay = delay;
			m_SizeTween = tweeny::from(m_Sprite->size.x, m_Sprite->size.y).to(to.x, to.y).during(time).via(__translate_easing(easing)).onStep([this](float x, float y) {
				m_Sprite->size.x = x;
				m_Sprite->size.y = y;
				return false;
			});
		}

		void TweenScale(const Vec2& to, float time, const Easing& easing, float delay)
		{
			m_ScaleTweenActive = true;
			m_ScaleTweenDelay = delay;
			m_ScaleTween = tweeny::from(m_Sprite->scale.x, m_Sprite->scale.y).to(to.x, to.y).during(time).via(__translate_easing(easing)).onStep([this](float x, float y) {
				m_Sprite->scale.x = x;
				m_Sprite->scale.y = y;
				return false;
			});
		}

		void TweenColor(const Color& to, float time, const Easing& easing, float delay)
		{
			m_ColorTweenActive = true;
			m_ColorTweenDelay = delay;
			m_ColorTween = tweeny::from(m_Sprite->color.r, m_Sprite->color.g, m_Sprite->color.b, m_Sprite->color.a).to(to.r, to.g, to.b, to.a).during(time).via(__translate_easing(easing)).onStep([this](float r, float g, float b, float a) {
				m_Sprite->color.r = r;
				m_Sprite->color.g = g;
				m_Sprite->color.b = b;
				m_Sprite->color.a = a;
				return false;
			});
		}

		void TweenRotation(float to, float time, const Easing& easing, float delay)
		{
			m_RotationTweenActive = true;
			m_RotationTweenDelay = delay;
			m_RotationTween = tweeny::from(m_Sprite->rotation).to(to).during(time).via(__translate_easing(easing)).onStep([this](float r) {
				m_Sprite->rotation = r;
				return false;
			});
		}

		void TweenOffset(const Vec2& to, float time, const Easing& easing, float delay)
		{
			m_OffsetTweenActive = true;
			m_OffsetTweenDelay = delay;
			m_OffsetTween = tweeny::from(m_Sprite->offset.x, m_Sprite->offset.y).to(to.x, to.y).during(time).via(__translate_easing(easing)).onStep([this](float x, float y) {
				m_Sprite->offset.x = x;
				m_Sprite->offset.y = y;
				return false;
			});
		}

		void SetValues(const AnimValues& values) override
		{
			m_Sprite->tex = values.tex;
			m_Sprite->size = values.size;
			m_Sprite->size.x = values.uvSize.x * values.tex->width;
			m_Sprite->size.y = values.uvSize.y * values.tex->height;
			m_Sprite->rotation = values.rotation;

			m_Sprite->offset = values.offset;
			m_Sprite->textureOffset = values.uvPos;
			m_Sprite->textureSize = values.uvSize;
			if (m_AutoAnimCenter)
			{
				m_Sprite->Center(m_AutoAnimCenterAxis, m_AutoAnimCenterContainerSize);
			}
		}

		const AnimValues GetValues() override
		{
			return {};
		}

		void AddAnimation(const std::string& name, Animation* animation)
		{
			m_AnimationManager.AddAnimation(name, animation);
		}

		void PlayAnimation(const std::string& animationName)
		{
			m_AnimationManager.PlayAnimation(animationName);
		}

		void AddAllAnimations(const std::map<std::string, Animation*> animations)
		{
			m_AnimationManager.AddAll(animations);
		}

		Animation* GetAnimation(const std::string& name)
		{
			return m_AnimationManager.GetAnimation(name);
		}

		void SetDefaultAnimation(Animation* animation)
		{
			m_AnimationManager.SetDefault(animation);
		}

		void SetDefaultAnimation(const std::string& name)
		{
			m_AnimationManager.SetDefault(name);
		}

		const Animation* GetCurrentAnimation() const
		{
			return m_AnimationManager.GetCurrentAnimation();
		}

		const std::string& GetCurrentAnimationName() const
		{
			return m_AnimationManager.GetCurrentAnimationName();
		}

		void CancelPositionTween()
		{
			if (!m_PosTweenActive) return;
			m_PosTweenActive = false;
		}

		void CancelSizeTween()
		{
			if (!m_SizeTweenActive) return;
			m_SizeTweenActive = false;
		}

		void CancelScaleTween()
		{
			if (!m_ScaleTweenActive) return;
			m_ScaleTweenActive = false;
		}

		void CancelRotationTween()
		{
			if (!m_RotationTweenActive) return;
			m_RotationTweenActive = false;
		}

		void EnableAutoAnimationCenter(glib::Axis axis, const Vec2& containerSize)
		{
			m_AutoAnimCenter = true;
			m_AutoAnimCenterAxis = axis;
			m_AutoAnimCenterContainerSize = containerSize;
		}

		void DisableAutoAnimationCenter()
		{
			m_AutoAnimCenter = false;
		}
	};
}

using namespace glib;

glib::Sprite::Sprite() : scale(Vec2(1.0f, 1.0f)), tex(nullptr), color({ 1.0f, 1.0f, 1.0f, 1.0f }), flipX(false), flipY(false), scrollFactor(Vec2(1.0f, 1.0f)), textureOffset(Vec2(0.0f, 0.0f)), textureSize(Vec2(1.0f, 1.0f))
{
	impl = new SpriteImpl(this);
	visible = true;
}

glib::Sprite::~Sprite()
{
	delete impl;
}

void glib::Sprite::Center(const Axis& axis, const Vec2& containerSize)
{
	switch (axis)
	{
	case Axis::X:
	{
		pos.x = containerSize.x / 2.0f - size.x / 2.0f;
		break;
	}
	case Axis::Y:
	{
		pos.y = containerSize.y / 2.0f - size.y / 2.0f;
		break;
	}
	case Axis::XY:
	{
		pos.x = containerSize.x / 2.0f - size.x / 2.0f;
		pos.y = containerSize.y / 2.0f - size.y / 2.0f;
		break;
	}
	}
}

void glib::Sprite::SetTexture(Texture* tex)
{
	size = Vec2(tex->width, tex->height);
	this->tex = tex;
}

void glib::Sprite::Draw()
{
	// Do nothing.
}

void glib::Sprite::Update(float delta)
{
	impl->Update(delta);
}

void glib::Sprite::TweenPosition(const Vec2& to, float time, const Easing& easing, float delay)
{
	impl->TweenPosition(to, time, easing, delay);
}

void glib::Sprite::TweenSize(const Vec2& to, float time, const Easing& easing, float delay)
{
	impl->TweenSize(to, time, easing, delay);
}

void glib::Sprite::TweenScale(const Vec2& to, float time, const Easing& easing, float delay)
{
	impl->TweenScale(to, time, easing, delay);
}

void glib::Sprite::TweenColor(const Color& to, float time, const Easing& easing, float delay)
{
	impl->TweenColor(to, time, easing, delay);
}

void glib::Sprite::TweenRotation(float to, float time, const Easing& easing, float delay)
{
	impl->TweenRotation(to, time, easing, delay);
}

void glib::Sprite::TweenOffset(const Vec2& to, float time, const Easing& easing, float delay)
{
	impl->TweenOffset(to, time, easing, delay);
}

void glib::Sprite::CancelPositionTween()
{
	impl->CancelPositionTween();
}

void glib::Sprite::CancelSizeTween()
{
	impl->CancelSizeTween();
}

void glib::Sprite::CancelScaleTween()
{
	impl->CancelScaleTween();
}

void glib::Sprite::CancelRotationTween()
{
	impl->CancelRotationTween();
}

void glib::Sprite::EnableAutoAnimationCenter(glib::Axis axis, const Vec2& containerSize)
{
	impl->EnableAutoAnimationCenter(axis, containerSize);
}

void glib::Sprite::DisableAutoAnimationCenter()
{
	impl->DisableAutoAnimationCenter();
}

void glib::Sprite::AddAnimation(const std::string& name, Animation* animation)
{
	impl->AddAnimation(name, animation);
}

void glib::Sprite::PlayAnimation(const std::string& animationName)
{
	impl->PlayAnimation(animationName);
}

void glib::Sprite::AddAllAnimations(const std::map<std::string, Animation*> animations)
{
	impl->AddAllAnimations(animations);
}

Animation* glib::Sprite::GetAnimation(const std::string& name)
{
	return impl->GetAnimation(name);
}

void glib::Sprite::SetDefaultAnimation(Animation* animation)
{
	impl->SetDefaultAnimation(animation);
}

void glib::Sprite::SetDefaultAnimation(const std::string& name)
{
	impl->SetDefaultAnimation(name);
}

const Animation* glib::Sprite::GetCurrentAnimation() const
{
	return impl->GetCurrentAnimation();
}

const std::string& glib::Sprite::GetCurrentAnimationName() const
{
	return impl->GetCurrentAnimationName();
}
