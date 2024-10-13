#pragma once

#include "../DLLDefs.h"
#include "Drawable.h"
#include "../math/Vec2.h"
#include "Texture.h"
#include "../utils/Easing.h"
#include "../math/Axis.h"
#include "../utils/Color.h"
#include "../animation/Animation.h"
#include "../physics/PhysicsObject.h"

#include <string>
#include <map>

namespace glib
{
	class SpriteImpl;

	class Sprite : public Drawable, public PhysicsObject
	{
	public:
		Vec2 size;
		Vec2 scale;
		Vec2 offset;
		Vec2 scrollFactor;
		Texture* tex;
		Color color;
		bool flipX;
		bool flipY;
		Vec2 textureOffset;
		Vec2 textureSize;
	private:
		SpriteImpl* impl;
	public:
		GLIB_API Sprite();
		GLIB_API ~Sprite();

		/**
		* Centers this sprite in the given container around the given axis.
		* 
		* @param axis[in] - The axis to center on
		* @param containerSize[in] - The container to center the sprite in (usually window->GetInitialSize())
		*/
		GLIB_API void Center(const Axis& axis, const Vec2& containerSize);

		/**
		* Sets the texture of this sprite to the given one and the size of this sprite to the of the given texture.
		* 
		* @param tex[in] - The new texture
		*/
		GLIB_API void SetTexture(Texture* tex);

		GLIB_API void Draw() override; // Does absolutley nothing.
		GLIB_API void Update(float delta) override; // Actually does something.

		/**
		* Tweens the position of this sprite.
		* 
		* @param to[in] - The end position
		* @param time[in] - The time the tween will take
		* @param easing[in] - The Easing of the tween
		*/
		GLIB_API void TweenPosition(const Vec2& to, float time, const Easing& easing, float delay = 0.0f);

		/**
		* Tweens the size of this sprite.
		*
		* @param to[in] - The end size
		* @param time[in] - The time the tween will take
		* @param easing[in] - The Easing of the tween
		*/
		GLIB_API void TweenSize(const Vec2& to, float time, const Easing& easing, float delay = 0.0f);

		/**
		* Tweens the scale of this sprite.
		*
		* @param to[in] - The end scale
		* @param time[in] - The time the tween will take
		* @param easing[in] - The Easing of the tween
		*/
		GLIB_API void TweenScale(const Vec2& to, float time, const Easing& easing, float delay = 0.0f);

		/**
		* Tweens the color of this sprite.
		*
		* @param to[in] - The end color
		* @param time[in] - The time the tween will take
		* @param easing[in] - The Easing of the tween
		*/
		GLIB_API void TweenColor(const Color& to, float time, const Easing& easing, float delay = 0.0f);

		/**
		* Tweens the rotation of this sprite.
		*
		* @param to[in] - The end rotation
		* @param time[in] - The time the tween will take
		* @param easing[in] - The Easing of the tween
		*/
		GLIB_API void TweenRotation(float to, float time, const Easing& easing, float delay = 0.0f);

		/**
		* Tweens the offset of this sprite.
		*
		* @param to[in] - The end offset
		* @param time[in] - The time the tween will take
		* @param easing[in] - The Easing of the tween
		*/
		GLIB_API void TweenOffset(const Vec2& to, float time, const Easing& easing, float delay = 0.0f);

		/**
		* Cancels the active position tween of this sprite.
		*/
		GLIB_API void CancelPositionTween();

		/**
		* Cancels the active size tween of this sprite.
		*/
		GLIB_API void CancelSizeTween();

		/**
		* Cancels the active scale tween of this sprite.
		*/
		GLIB_API void CancelScaleTween();

		/**
		* Cancels the active rotation tween of this sprite.
		*/
		GLIB_API void CancelRotationTween();

		GLIB_API void EnableAutoAnimationCenter(glib::Axis axis, const Vec2& containerSize);
		GLIB_API void DisableAutoAnimationCenter();

		// See AnimationManager

		GLIB_API void AddAnimation(const std::string& name, Animation* animation);
		GLIB_API void PlayAnimation(const std::string& animationName);
		GLIB_API void AddAllAnimations(const std::map<std::string, Animation*> animations);
		GLIB_API Animation* GetAnimation(const std::string& name);
		GLIB_API void SetDefaultAnimation(Animation* animation);
		GLIB_API void SetDefaultAnimation(const std::string& name);
		GLIB_API const Animation* GetCurrentAnimation() const;
		GLIB_API const std::string& GetCurrentAnimationName() const;
	};
}