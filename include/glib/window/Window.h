#pragma once

#include "../DLLDefs.h"
#include "../math/Vec2.h"
#include "../graphics/pipeline/RenderPipeline.h"
#include "../graphics/Shader.h"
#include "../graphics/Texture.h"
#include "../event/EventManager.h"
#include "../sound/SoundManager.h"
#include "../math/Vec2.h"
#include "../graphics/Font.h"
#include "glib/apkg/apkg.h"
#include "../graphics/3d/Model.h"

#include <string>

namespace glib
{
	class WindowImpl;
	class Instance;
	struct WindowInitParams;

	/**
	* The window class is the core of any glib game. It manages everything from rendering to sound and assets.
	* It can be seen as a context.
	* 
	* Every asset loaded with the window class will be automatically freed on close.
	* Cameras added to the window will also be automatically freed on close.
	*/
	class Window
	{
	private:
		WindowImpl* impl;
	public:
		Window(Instance* instance, const std::string& title, int width, int height, const WindowInitParams& params); // Internal
		~Window(); // Internal

		/**
		* Hides or shows the window.
		* 
		* @param visible - Wether to show or hide the window
		*/
		GLIB_API void SetVisible(bool visible);

		/**
		* Returns the current size of the window that the user sees.
		* 
		* @returns The current size of the window
		*/
		GLIB_API const Vec2 GetSize() const;

		/**
		* Returns the size that was specified on window creation.
		* 
		* @returns The initial size of the window
		*/
		GLIB_API const Vec2 GetInitialSize() const;

		/**
		* Returns the current RenderPipeline.
		* 
		* @returns The current RenderPipeline
		*/
		GLIB_API RenderPipeline* GetRenderPipeline();

		/**
		* Draws everything to the screen by invoking RenderPipeline::Flush and doing some preperations.
		*/
		GLIB_API void Draw() const;

		/**
		* Returns wether the window is open or closed. A substitution for glfwWindowShouldClose
		* 
		* @returns Wether the window is open or closed
		*/
		GLIB_API bool IsOpen() const;

		/**
		* Updates things like tweens, animations and other similar things.
		*/
		GLIB_API void Update(float delta);

		/**
		* Updates the EventManager and processes window events
		*/
		GLIB_API void UpdateEvents(float delta);

		/**
		* Returns the default camera. The default camera is just index 0 of the camera array.
		* 
		* @returns The default camera
		*/
		GLIB_API Camera* GetDefaultCamera();

		/**
		* Returns the EventManager.
		* 
		* @returns The EventManager
		*/
		GLIB_API EventManager& GetEventManager();

		/**
		* Returns the SoundManager.
		*
		* @returns The SoundManager
		*/
		GLIB_API SoundManager& GetSoundManager();

		/**
		* Returns the viewport position that is calculated when the window is resized.
		* 
		* @returns The viewport position.
		*/
		GLIB_API const Vec2& GetViewportPos();

		/**
		* Returns the viewport size that is calculated when the window is resized.
		*
		* @returns The viewport size.
		*/
		GLIB_API const Vec2& GetViewportSize();

		/**
		* Compiles a shader program.
		* 
		* To load a shader from files use: Window::LoadShaderFromFiles.
		* 
		* @param vertexCode - The code for the vertex shader
		* @param fragmentCode - The code for the fragment shader
		* 
		* @returns A shader program compiled from the two provided shaders
		*/
		GLIB_API Shader* LoadShader(const std::string& vertexCode, const std::string& fragmentCode);

		/**
		* Loads a shader from files.
		* 
		* @see Window::LoadShader
		* 
		* @param vertexPath - The path for the vertex shader code
		* @param fragmentPath - The path for the fragment shader code
		* 
		* @returns A shader program compiled from the two provided shaders 
		*/
		GLIB_API Shader* LoadShaderFromFiles(const std::string& vertexPath, const std::string& fragmentPath);

		/**
		* Loads a texture from an image file.
		* 
		* @param path - The path to the image file
		* @param pixelart - Wether the image should be treated as pixel art (disables antialiasing)
		* 
		* @returns A texture loaded from an image file
		*/
		GLIB_API Texture* LoadTexture(const std::string& path, bool pixelart = false);

		/**
		* Loads a texture from a .apkg package file.
		*
		* @param path - The path to the image file
		* @param pixelart - Wether the image should be treated as pixel art (disables antialiasing)
		*
		* @returns A texture loaded from an image file
		*/
		GLIB_API Texture* LoadTextureFromPackage(const std::string& packagePath, const std::string& path, bool pixelart = false);

		/**
		* Loads the raw image data as a pixel array from a file into memory.
		* 
		* @param path - The path to the image file
		* 
		* @returns Raw image data
		*/
		GLIB_API ImageData LoadTextureRaw(const std::string& path);

		/**
		* Converts raw image data to a texture that can be used for rendering.
		* 
		* @param data - The image data
		* @param pixelart - Wether the image should be treated as pixel art (disables antialiasing)
		* 
		* @returns A texture that can be used for rendering
		*/
		GLIB_API Texture* LoadTextureFromRawData(ImageData data, bool pixelart = false);

		/**
		* Loads a font from a file (.ttf). The default ASCII charset will be used.
		* 
		* @param path - The path to the font file
		* @param size - The size of the font
		* @param pixelart - Wether the font should be treated as pixel art (disables antialiasing)
		* 
		* @returns A font that can be used to render text
		*/
		GLIB_API Font* LoadFont(const std::string& path, int size = 48, bool pixelart = false);

		/**
		* Loads a font from a file (.ttf) and the provided characters as renderable glyphs.
		*
		* @param path - The path to the font file
		* @param charset - An array of wchars (16 bit / short) to be loaded
		* @param charsetLen - The length of the charset array (how many characters)
		* @param size - The size of the font
		* @param pixelart - Wether the font should be treated as pixel art (disables antialiasing)
		*
		* @returns A font that can be used to render text
		*/
		GLIB_API Font* LoadFont(const std::string& path, wchar_t* charset, size_t charsetLen, int size = 48, bool pixelart = false);

		/**
		* Loads a font from a file (.ttf) and the provided characters as renderable glyphs.
		*
		* @param path - The path to the font file
		* @param charset - An array of wchars (16 bit / short) to be loaded
		* @param charsetLen - The length of the charset array (how many characters)
		* @param size - The size of the font
		* @param pixelart - Wether the font should be treated as pixel art (disables antialiasing)
		*
		* @returns A font that can be used to render text
		*/
		GLIB_API Font* LoadFontFromPackage(const std::string& packagePath, const std::string& path, int size = 48, bool pixelart = false);

		/**
		* Loads a font from a file (.ttf) and the provided characters as renderable glyphs.
		*
		* @param path - The path to the font file
		* @param charset - An array of wchars (16 bit / short) to be loaded
		* @param charsetLen - The length of the charset array (how many characters)
		* @param size - The size of the font
		* @param pixelart - Wether the font should be treated as pixel art (disables antialiasing)
		*
		* @returns A font that can be used to render text
		*/
		GLIB_API Font* LoadFontFromPackage(const std::string& packagePath, const std::string& path, wchar_t* charset, size_t charsetLen, int size = 48, bool pixelart = false);

		GLIB_API Model* LoadModel(const std::string& path, bool pixelart = false);

		/**
		* Enables the OpenGL and GLFW context of this window for this thread.
		* @see glfwMakeContextCurrent
		*/
		GLIB_API void SetToCurrentContext();

		/**
		* Disables the OpenGL and GLFW context of this window for this thread.
		* @see glfwMakeContextCurrent
		*/
		GLIB_API void SetNotToCurrentContext();

		/**
		* Hides the cursor when it enters this window.
		*/
		GLIB_API void HideCursor();

		/**
		* Shows shows the cursor when it enters this window.
		*/
		GLIB_API void ShowCursor();

		GLIB_API void LockCursor();
		GLIB_API void UnlockCursor();

		/**
		* Sets the icon of this window.
		* 
		* @param path[in] - The path to the icon file (Image File)
		*/
		GLIB_API void SetIcon(const std::string& path);

		/**
		* Sets the icon of this window.
		*
		* * @param packagePath[in] - The path to package file (.apkg File)
		* @param path[in] - The name of the image file
		*/
		GLIB_API void SetIconFromPackage(const std::string& packagePath, const std::string& path);

		/**
		* Sets the RenderPipeline to the provided one.
		* @see Instance::CreateDefaultPipeline
		* @see RenderPipeline
		*/
		GLIB_API void SetRenderPipeline(RenderPipeline* pipeline);

		/**
		* Added cameras will be automatically deleted when the window closes.
		*/
		GLIB_API void AddCamera(Camera* camera);
		GLIB_API void RemoveCamera(Camera* camera);

		/**
		* Returns the static camera of this window. The static camera will always be rendered last, so that its like an overlay.
		* @returns The static camera of this window.
		*/
		GLIB_API Camera* GetStaticCamera();

		/**
		* Not recommended to use!
		* 
		* This method will remove all objects (e.g. Textures, Fonts) from the internal caches. These objects will not be deleted when the window closes.
		*/
		GLIB_API void ClearCaches();

		/**
		* Not recommended to use!
		* 
		* This method will remove and delete all objects (e.g. Textures, Fonts) from the internal caches.
		*/
		GLIB_API void ClearCachesDelete();

		GLIB_API void SetFullscreen(bool enabled);

		void* GetHandle();
	};
}