#pragma once

#include "../DLLDefs.h"
#include "../graphics/Texture.h"
#include "glib/math/Vec2.h"

namespace glib
{
	class AnimationImpl;

	/**
	* A Structure that holds the texture and offsets of an animation frame.
	*/
	struct GLIB_API AnimValues
	{
		Texture* tex;
		Vec2 offset;
		Vec2 uvPos;
		Vec2 uvSize;
		Vec2 size;
		float rotation;
	};

	/**
	* A class that can be extended to make something animatable (like a sprite).
	*/
	class Animatable
	{
	protected:
		/**
		* Sets the values of the passed frame.
		* 
		* @param values[in] - The frame
		*/
		GLIB_API virtual void SetValues(const AnimValues& values) = 0;

		/**
		* Returns the values set by the last SetValues call.
		* 
		* @returns A AnimValues struct containing the last values.
		*/
		GLIB_API virtual const AnimValues GetValues() = 0;

		friend class AnimationImpl;
	};
}