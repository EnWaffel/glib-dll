#include <glad/glad.h>
#include "glib/backend/OpenGLProxy.h"


void glib::OpenGLProxy::Enable(int glenum)
{
	glEnable(glenum);
}

void glib::OpenGLProxy::BlendFunc(int glenum1, int glenum2)
{
	glBlendFunc(glenum1, glenum2);
}

void glib::OpenGLProxy::Viewport(int x, int y, int w, int h)
{
	glViewport(x, y, w, h);
}

void glib::OpenGLProxy::ClearColor(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

void glib::OpenGLProxy::Clear(int bufferbits)
{
	glClear(bufferbits);
}

void glib::OpenGLProxy::ActiveTexture(int glenum)
{
	glActiveTexture(glenum);
}

void glib::OpenGLProxy::DrawArrays(int glenum, int first, int count)
{
	glDrawArrays(glenum, first, count);
}

void glib::OpenGLProxy::BindVertexArray(int vao)
{
	glBindVertexArray(vao);
}
