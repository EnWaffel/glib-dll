#include "glib/framebuffer/Framebuffer.h"
#include "glib/glibError.h"

#include <glad/glad.h>

extern int __GLIB_ERROR_CODE;
extern void glib_print_error();

namespace glib
{
	class FramebufferImpl
	{
	private:
		unsigned int m_FBO;
		unsigned int m_TexID;
		unsigned int m_RBO;
	public:
		FramebufferImpl(FramebufferType type, const Vec2& dimensions) : m_FBO(0), m_TexID(0), m_RBO(0)
		{
			glGenFramebuffers(1, &m_FBO);
			glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

			if (type == FramebufferType::TEXTURE)
			{
				glGenTextures(1, &m_TexID);
				glBindTexture(GL_TEXTURE_2D, m_TexID);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dimensions.x, dimensions.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

				if (GL_EXT_texture_filter_anisotropic) {
					GLfloat largest;
					glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largest);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largest);
				}

				glBindTexture(GL_TEXTURE_2D, 0);

				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TexID, 0);
			}
			else
			{
				__GLIB_ERROR_CODE = GLIB_FRAMEBUFFER_UNKNOWN_TYPE;
				glib_print_error();
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				glDeleteFramebuffers(1, &m_FBO);
				return;
			}

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			{
				__GLIB_ERROR_CODE = GLIB_FRAMEBUFFER_NOT_COMPLETE;
				glib_print_error();
				return;
			}
			
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		~FramebufferImpl()
		{
			glDeleteFramebuffers(1, &m_FBO);
			if (m_TexID != 0)
			{
				glDeleteTextures(1, &m_TexID);
			}
		}

		void Bind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		}

		void Unbind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		FBInternals GetInternals()
		{
			return { m_TexID };
		}
	};
}

using namespace glib;

glib::Framebuffer::Framebuffer(FramebufferType type, const Vec2& dimensions)
{
	impl = new FramebufferImpl(type, dimensions);
}

glib::Framebuffer::~Framebuffer()
{
	delete impl;
}

void glib::Framebuffer::Bind()
{
	impl->Bind();
}

void glib::Framebuffer::Unbind()
{
	impl->Unbind();
}

FBInternals glib::Framebuffer::GetInternals()
{
	return impl->GetInternals();
}
