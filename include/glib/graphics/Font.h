#pragma once

#include "../DLLDefs.h"
#include "../math/Vec2.h"

#include <string>

namespace glib
{
	/**
	* A Glyph or Character
	*/
	struct GLIB_API Glyph
	{
		unsigned int tex;
		Vec2 size;
		Vec2 bearing;
		unsigned int advance;
	};

	class FontImpl;

	class Font
	{
	private:
		FontImpl* impl;
	public:
		Font(const std::string& path, wchar_t* alphabet, size_t alphabetLen, int size, bool pixelart); // Internal
		Font(const std::string& packagePath, const std::string& path, wchar_t* alphabet, size_t alphabetLen, int size, bool pixelart); // Internal
		~Font(); // Internal

		/**
		* Returns the data of a glyph.
		* 
		* @param c[in] - A Glyph
		* 
		* @returns the data the specified glyph if loaded, otherwise an empty struct
		*/
		GLIB_API const Glyph& GetGlyph(wchar_t c);

		/**
		* Calculates the width in pixels of text on screen.
		* 
		* @param text[in] - The text to calculate the width from
		* @param scale[in] - The scale of the text
		*/
		GLIB_API float CalculateWidth(const std::wstring& text, float scale = 1.0f);

		/**
		* Calculates the height in pixels of text on screen.
		*
		* @param text[in] - The text to calculate the height from
		* @param scale[in] - The scale of the text
		*/
		GLIB_API float CalculateHeight(const std::wstring& text, float scale = 1.0f);
	};

}