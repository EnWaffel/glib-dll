#include "glib/graphics/Texture.h"
#include <glad/glad.h>
#include <iostream>

namespace glib
{
	class TextureImpl
	{
	private:
		unsigned int m_ID;
		bool m_DeleteID = true;
	public:
		TextureImpl(unsigned int id) : m_ID(id)
		{
		}

		~TextureImpl()
		{
			if (!m_DeleteID) return;
			glDeleteTextures(1, &m_ID);
		}

		void Bind()
		{
			glBindTexture(GL_TEXTURE_2D, m_ID);
		}

		void Unbind()
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		unsigned int GetID()
		{
			return m_ID;
		}

		void SetDeleteID(bool deleteID)
		{
			m_DeleteID = deleteID;
		}
	};
}

using namespace glib;

glib::Texture::Texture(unsigned int id, int width, int height) : width(width), height(height)
{
	impl = new TextureImpl(id);
}

glib::Texture::~Texture()
{
	delete impl;
}

void glib::Texture::Bind()
{
	impl->Bind();
}

void glib::Texture::Unbind()
{
	impl->Unbind();
}

unsigned int glib::Texture::GetID()
{
	return impl->GetID();
}

void glib::Texture::SetDeleteID(bool deleteID)
{
	impl->SetDeleteID(deleteID);
}
