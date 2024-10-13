#include "glib/graphics/Font.h"
#include "glib/apkg/apkg.h"

#include <freetype/freetype.h>
#include <glad/glad.h>
#include <unordered_map>
#include <iostream>

namespace glib
{
	class FontImpl
	{
	private:
		std::unordered_map<wchar_t, Glyph> m_Glyphs;
	public:
		FontImpl(const std::string& path, wchar_t* alphabet, size_t alphabetLen, int size, bool pixelart)
		{
            FT_Library ft;
            if (FT_Init_FreeType(&ft))
            {
                std::cout << "Failed to init FreeType!" << std::endl;
                return;
            }

            FT_Face face;
            if (FT_New_Face(ft, path.c_str(), 0, &face))
            {
                std::cout << "Failed to load file!" << std::endl;
                return;
            }

            FT_Select_Charmap(face, FT_ENCODING_UNICODE);
            FT_Set_Pixel_Sizes(face, 0, size);

            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            for (int i = 0; i < alphabetLen; i++)
            {
                FT_UInt glyph_index = FT_Get_Char_Index(face, alphabet[i]);

                FT_Load_Glyph(face, glyph_index, FT_LOAD_RENDER);

                unsigned int texture = 0;
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);
                glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RED,
                    face->glyph->bitmap.width,
                    face->glyph->bitmap.rows,
                    0,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    face->glyph->bitmap.buffer
                );

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                
                if (pixelart)
                {
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                }
                else
                {
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                }

                if (GL_EXT_texture_filter_anisotropic) {
                    GLfloat largest = 0;
                    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largest);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largest);
                }

                Glyph glyph = {
                    texture,
                    Vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    Vec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    face->glyph->advance.x
                };

                m_Glyphs.insert({ alphabet[i], glyph });
            }

            FT_Done_Face(face);
            FT_Done_FreeType(ft);
		}

        FontImpl(const std::string& packagePath, const std::string& path, wchar_t* alphabet, size_t alphabetLen, int size, bool pixelart)
        {
            FT_Library ft;
            if (FT_Init_FreeType(&ft))
            {
                std::cout << "Failed to init FreeType!" << std::endl;
                return;
            }

            apkg::FileData fd = apkg::UnpackOnce(packagePath, path);

            FT_Face face;
            if (FT_New_Memory_Face(ft, (const FT_Byte*)fd.buf, fd.bufLen, 0, &face))
            {
                std::cout << "Failed to load file!" << std::endl;
                return;
            }

            FT_Select_Charmap(face, FT_ENCODING_UNICODE);
            FT_Set_Pixel_Sizes(face, 0, size);

            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            for (int i = 0; i < alphabetLen; i++)
            {
                FT_UInt glyph_index = FT_Get_Char_Index(face, alphabet[i]);

                FT_Load_Glyph(face, glyph_index, FT_LOAD_RENDER);

                unsigned int texture = 0;
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);
                glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RED,
                    face->glyph->bitmap.width,
                    face->glyph->bitmap.rows,
                    0,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    face->glyph->bitmap.buffer
                );

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

                if (pixelart)
                {
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                }
                else
                {
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                }

                if (GL_EXT_texture_filter_anisotropic) {
                    GLfloat largest = 0;
                    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largest);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largest);
                }

                Glyph glyph = {
                    texture,
                    Vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    Vec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    face->glyph->advance.x
                };

                m_Glyphs.insert({ alphabet[i], glyph });
            }

            FT_Done_Face(face);
            FT_Done_FreeType(ft);
            delete[] fd.buf;
        }

		~FontImpl()
		{
            for (const auto& v : m_Glyphs)
            {
                glDeleteTextures(1, &v.second.tex);
            }
		}

        const Glyph& GetGlyph(wchar_t c)
        {
            try
            {
                return m_Glyphs.at(c);
            }
            catch (std::exception e)
            {
                return m_Glyphs.at(0);
            }
        }
	};
}

using namespace glib;

glib::Font::Font(const std::string& path, wchar_t* alphabet, size_t alphabetLen, int size, bool pixelart)
{
	impl = new FontImpl(path, alphabet, alphabetLen, size, pixelart);
}

glib::Font::Font(const std::string& packagePath, const std::string& path, wchar_t* alphabet, size_t alphabetLen, int size, bool pixelart)
{
    impl = new FontImpl(packagePath, path, alphabet, alphabetLen, size, pixelart);
}

glib::Font::~Font()
{
	delete impl;
}

const Glyph& glib::Font::GetGlyph(wchar_t c)
{
    return impl->GetGlyph(c);
}

float glib::Font::CalculateWidth(const std::wstring& text, float scale)
{
    float width = 0.0f;

    for (wchar_t c : text)
    {
        const Glyph& glyph = GetGlyph(c);
        width += (glyph.advance >> 6) * scale;
    }

    return width;
}

float glib::Font::CalculateHeight(const std::wstring& text, float scale)
{
    float height = 0.0f;

    for (wchar_t c : text)
    {
        const Glyph& glyph = GetGlyph(c);
        if (glyph.size.y * scale > height)
        {
            height = glyph.size.y * scale;
        }
    }

    return height;
}
