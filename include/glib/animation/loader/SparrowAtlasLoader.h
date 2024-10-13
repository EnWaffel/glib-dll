#pragma once

#include "../../DLLDefs.h"
#include "../Animation.h"
#include "../../window/Window.h"

#include <string>
#include <map>

namespace glib
{
	class SparrowAtlasLoader
	{
	public:
		/**
		* Loads all the animations from a Sparrow Atlas. All animations will be loaded with 24 fps.
		* 
		* @param path[in] - The path to the xml file
		* @param imagePath[in] - The path to the image file
		* @param wnd[in] - The window the animations will be loaded for
		* @param pixelart[in] - Wether all animations should be loaded as pixel art
		* @param xFlipped[in] - Wether all animations should be loaded horizontally flipped
		* 
		* @returns an AnimationTable containing the loaded animations
		*/
		GLIB_API static std::map<std::string, Animation*> LoadFile(const std::string& path, const std::string& imagePath, Window* wnd, bool pixelart, bool xFlipped);
		/**
		* Loads all the animations from a Sparrow Atlas.
		*
		* @param path[in] - The path to the xml file
		* @param imagePath[in] - The path to the image file
		* @param wnd[in] - The window the animations will be loaded for
		* @param pixelart[in] - Wether all animations should be loaded as pixel art
		* @param xFlipped[in] - Wether all animations should be loaded horizontally flipped
		* @param fpsMap[in] - A map containing the fps count for the animations. if no fps count for an animation is provided, 24 will be used
		*
		* @returns an AnimationTable containing the loaded animations
		*/
		GLIB_API static std::map<std::string, Animation*> LoadFile(const std::string& path, const std::string& imagePath, Window* wnd, bool pixelart, bool xFlipped, const std::map<std::string, int>& fpsMap);
	};
}