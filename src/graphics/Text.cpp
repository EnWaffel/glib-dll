#include "glib/graphics/Text.h"

#include <vector>
#include <tweeny.h>

extern tweeny::easing::enumerated __translate_easing(glib::Easing easing);

namespace glib
{
	class TextImpl
	{
	private:
		Text* m_Txt;
		std::wstring m_Text;
		std::vector<Glyph> m_Glyphs;
		bool m_AutoCenterToggle;
		Axis m_AutoCenterAxis;
		Vec2 m_AutoCenterContainerSize;
		tweeny::tween<float, float> m_PosTween;
		bool m_PosTweenActive = false;
		tweeny::tween<float> m_ScaleTween;
		bool m_ScaleTweenActive = false;
		tweeny::tween<float> m_RotationTween;
		bool m_RotationTweenActive = false;
		tweeny::tween<float, float, float, float> m_ColorTween;
		bool m_ColorTweenActive = false;

		float m_PosTweenDelay = 0.0f;
		float m_ScaleTweenDelay = 0.0f;
		float m_RotationTweenDelay = 0.0f;
		float m_ColorTweenDelay = 0.0f;
	public:
		TextImpl(Text* txt) : m_Txt(txt), m_AutoCenterToggle(false)
		{
		}

		~TextImpl()
		{
		}

		void SetText(const std::wstring& text)
		{
			m_Text = text;
			m_Txt->size = glib::Vec2(m_Txt->font->CalculateWidth(text, m_Txt->scale), m_Txt->font->CalculateHeight(text, m_Txt->scale));
			if (m_AutoCenterToggle)
			{
				m_Txt->Center(m_AutoCenterAxis, m_AutoCenterContainerSize);
			}
		}

		const std::wstring& GetText()
		{
			return m_Text;
		}

		void SetAutoCenter(bool toggle, Axis axis, const Vec2& containerSize)
		{
			m_AutoCenterToggle = toggle;
			m_AutoCenterAxis = axis;
			m_AutoCenterContainerSize = containerSize;
		}

		void Update(float delta)
		{
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
		}

		void TweenPosition(const Vec2& to, float time, const Easing& easing, float delay)
		{
			m_PosTweenActive = true;
			m_PosTweenDelay = delay;
			m_PosTween = tweeny::from(m_Txt->pos.x, m_Txt->pos.y).to(to.x, to.y).during(time).via(__translate_easing(easing)).onStep([this](float x, float y) {
				m_Txt->pos.x = x;
				m_Txt->pos.y = y;
				return false;
			});
		}

		void TweenScale(float to, float time, const Easing& easing, float delay)
		{
			m_ScaleTweenActive = true;
			m_ScaleTweenDelay = delay;
			m_ScaleTween = tweeny::from(m_Txt->scale).to(to).during(time).via(__translate_easing(easing)).onStep([this](float s) {
				m_Txt->scale = s;
				return false;
			});
		}

		void TweenRotation(float to, float time, const Easing& easing, float delay)
		{
			m_RotationTweenActive = true;
			m_RotationTweenDelay = delay;
			m_RotationTween = tweeny::from(m_Txt->rotation).to(to).during(time).via(__translate_easing(easing)).onStep([this](float r) {
				m_Txt->rotation = r;
				return false;
			});
		}

		void TweenColor(const Color& to, float time, const Easing& easing, float delay)
		{
			m_ColorTweenActive = true;
			m_ColorTweenDelay = delay;
			m_ColorTween = tweeny::from(m_Txt->color.r, m_Txt->color.g, m_Txt->color.b, m_Txt->color.a).to(to.r, to.g, to.b, to.a).during(time).via(__translate_easing(easing)).onStep([this](float r, float g, float b, float a) {
				m_Txt->color.r = r;
				m_Txt->color.g = g;
				m_Txt->color.b = b;
				m_Txt->color.a = a;
				return false;
			});
		}
	};
}

using namespace glib;

glib::Text::Text() : scale(1.0f), font(nullptr), color({ 1.0f, 1.0f, 1.0f, 1.0f }), xFlip(false), yFlip(false), rotation(0.0f)
{
	visible = true;
	impl = new TextImpl(this);
}

glib::Text::~Text()
{
	delete impl;
}

void glib::Text::Center(const Axis& axis, const Vec2& containerSize)
{
	switch (axis)
	{
	case Axis::X:
	{
		pos.x = containerSize.x / 2.0f - font->CalculateWidth(GetText(), scale) / 2.0f;
		break;
	}
	case Axis::Y:
	{
		pos.y = containerSize.y / 2.0f - font->CalculateHeight(GetText(), scale) / 2.0f;
		break;
	}
	case Axis::XY:
	{
		pos.x = containerSize.x / 2.0f - font->CalculateWidth(GetText(), scale) / 2.0f;
		pos.y = containerSize.y / 2.0f - font->CalculateHeight(GetText(), scale) / 2.0f;
		break;
	}
	}
}

void glib::Text::Draw()
{
}

void glib::Text::Update(float delta)
{
	impl->Update(delta);
}

void glib::Text::SetText(const std::wstring& text)
{
	impl->SetText(text);
}

const std::wstring& glib::Text::GetText()
{
	return impl->GetText();
}

void glib::Text::SetAutoCenter(bool toggle, Axis axis, const Vec2& containerSize)
{
	impl->SetAutoCenter(toggle, axis, containerSize);
}

void glib::Text::TweenPosition(const Vec2& to, float time, const Easing& easing, float delay)
{
	impl->TweenPosition(to, time, easing, delay);
}

void glib::Text::TweenScale(float to, float time, const Easing& easing, float delay)
{
	impl->TweenScale(to, time, easing, delay);
}

void glib::Text::TweenRotation(float to, float time, const Easing& easing, float delay)
{
	impl->TweenRotation(to, time, easing, delay);
}

void glib::Text::TweenColor(const Color& to, float time, const Easing& easing, float delay)
{
	impl->TweenColor(to, time, easing, delay);
}
