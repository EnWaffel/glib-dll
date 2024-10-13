#pragma once

#include "../DLLDefs.h"
#include "../graphics/Texture.h"
#include "Animatable.h"
#include "../math/Vec2.h"

#include <vector>
#include <string>
#include <map>

namespace glib
{

	/**
	* A Structure that holds the texture and offsets of an animation frame.
	*/
	struct GLIB_API AnimationFrame
	{
		Vec2 offset;
		Vec2 uvPos;
		Vec2 uvSize;
		Vec2 size;
		float rotation;
	};

	class AnimationImpl;

	/**
	* An animation that can be played.
	*/
	class Animation
	{
	private:
		AnimationImpl* impl;
	public:
		GLIB_API Animation(std::vector<AnimationFrame> frames, Texture* tex, int fps, bool loop, int* refCount);
		GLIB_API ~Animation();

		/**
		* Play this animation on the specified Animatable.
		* 
		* @param obj[in] - The object to be animated.
		*/
		GLIB_API void Play(Animatable* obj) const;

		/**
		* Pauses the animation.
		*/
		GLIB_API void Pause() const;

		/**
		* Resumes the animation.
		*/
		GLIB_API void Resume() const;

		/**
		* Stops the animation.
		*/
		GLIB_API void Stop() const;

		/**
		* @returns true if the animation finished playing, otherwise false
		*/
		GLIB_API bool IsFinished() const;

		/**
		* Sets the fps of this animation.
		* 
		* @param fps[in] - The frames per second
		*/
		GLIB_API void SetFPS(int fps);

		/**
		* Sets wether this animation should loop.
		*
		* @param looping[in] - Wether this animation should loop
		*/
		GLIB_API void SetLooping(bool looping);

		/**
		* Sets the position offset of this animation.
		*
		* @param offset[in] - The position offset
		*/
		GLIB_API void SetOffset(const Vec2& offset);

		void Update(float delta); // Internal

		/**
		* Allocates a new AnimationTable and copies all the data from the given table. Don't forget to free the returned
		* table by calling FreeTable().
		* 
		* @param table[in] - The table to be copied
		* 
		* @returns A newly allocated AnimationTable containing the same data as the original table
		*/
		GLIB_API static std::map<std::string, Animation*> CopyTable(const std::map<std::string, Animation*>& table);

		/**
		* Frees an animation table.
		* 
		* @param table[in] - The table to be freed
		* @param alsoTextures[in] - Wether the textures of all the animation frames contained in this table should be deleted.
		*/
		GLIB_API static void FreeTable(const std::map<std::string, Animation*>& table);

		GLIB_API static Animation* Copy(Animation* anim);
	};

	typedef std::map<std::string, Animation*> AnimationTable;
}