#pragma once

#include "../DLLDefs.h"
#include "Animatable.h"
#include "Animation.h"

#include <string>
#include <map>

namespace glib
{
	class AnimationManagerImpl;

	/**
	* A class that manages animations for Animatables. An example of this is the Sprite class.
	*/
	class AnimationManager
	{
	private:
		AnimationManagerImpl* impl;
	public:
		/**
		* @param obj[in] - The object that holds this animation manager.
		*/
		GLIB_API AnimationManager(Animatable* obj);
		GLIB_API ~AnimationManager();

		/**
		* Plays an animation with the corresponding name.
		* 
		* @param animationName[in] - The animation name.
		*/
		GLIB_API void PlayAnimation(const std::string& animationName);

		/**
		* Adds an animation to this manager.
		* 
		* @param name[in] - The name for the animation
		* @param animation[in] - The animation object
		*/
		GLIB_API void AddAnimation(const std::string& name, Animation* animation);

		/**
		* Adds an AnimationTable to this manager.
		* 
		* @param animations[in] - The AnimationTable
		*/
		GLIB_API void AddAll(const std::map<std::string, Animation*> animations);

		/**
		* Returns an animation with the corresponding name.
		* 
		* @param name[in] - The name of an animation
		* 
		* @returns an animation with the corresponding name
		*/
		GLIB_API Animation* GetAnimation(const std::string& name);

		/**
		* Sets the default animation. The default animation will always be played when any other animation finished playing.
		* Basically an idle animation.
		* 
		* @param animation[in] - The animation object to be set as default
		*/
		GLIB_API void SetDefault(Animation* animation);

		/**
		* Sets the default animation. The default animation will always be played when any other animation finished playing.
		* Basically an idle animation.
		*
		* @param name[in] - The name of an animation to be set as default (an animation with the name must already be added to this manager)
		*/
		GLIB_API void SetDefault(const std::string& name);

		/**
		* @returns The current playing animation, otherwise nullptr
		*/
		GLIB_API const Animation* GetCurrentAnimation() const;

		/**
		* @returns The current playing animation name, otherwise an empty string
		*/
		GLIB_API const std::string& GetCurrentAnimationName() const;

		/**
		* Updates the manager and any playing animation.
		*/
		GLIB_API void Update(float delta);
	};
}