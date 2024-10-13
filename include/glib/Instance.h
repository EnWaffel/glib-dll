#pragma once

#include <string>

#include "DLLDefs.h"
#include "window/Window.h"
#include "glibError.h"

namespace glib
{
	struct WindowInitParams
	{
		bool manageAssets = true; // Turning this flag off is not recommended
		bool noDecorations = false;
	};

	class InstanceImpl;

	/**
	* The Instance class holds all windows in a program and has a few utility functions.
	*/
	class Instance
	{
	private:
		InstanceImpl* impl;
	public:
		Instance(); // Internal
		~Instance(); // Internal

		/**
		* This function creates a window with the given title and dimensions.
		* A window can be seen as a context. You can build your entire game from this class (Window class).
		* 
		* NOTE: The width and height args will be used during your entire game as the size for cameras,
		* and viewable space.
		* 
		* @param title[in] - The title of the window
		* @param width[in] - The initial width of the window
		* @param height[in] - The initial height of the window
		* @param params[in] - Parameters for window creation
		*/
		GLIB_API Window* CreateWindow(const std::string& title, int width, int height, const WindowInitParams& params = {});

		/**
		* A utility function for creating a basic render pipeline.
		* Only includes CameraRenderer and WindowRenderer.
		* 
		* @param wnd - The window to create the pipeline for.
		* 
		* @returns The created pipeline. (with ownership)
		*/
		GLIB_API RenderPipeline* CreateDefaultPipeline(Window* wnd) const;

		/**
		* Returns the current time since glib was initialized in seconds.
		* Basically glfwGetTime.
		* 
		* @returns The current time in seconds.
		*/
		GLIB_API double GetTime() const;
	};
}