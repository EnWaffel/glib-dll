#include "glib/graphics/pipeline/PipelineRenderer.h"
#include "glib/graphics/camera/Camera.h"
#include "glib/window/Window.h"

#include <glad/glad.h>

static const char* VERTEX_SHADER = R"(
#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 _texCoord;

uniform mat4 glib_projection_matrix;
uniform mat4 glib_view_matrix;
uniform mat4 glib_model_matrix;

out vec2 glib_uv;

void main()
{
	gl_Position = glib_projection_matrix * glib_view_matrix * glib_model_matrix * vec4(pos, 1.0);
	glib_uv = _texCoord;
}
)";

namespace glib
{
	static class RendererListener : public EventSubscriber
	{
	private:
		PipelineRenderer* m_R;
	public:
		RendererListener(PipelineRenderer* r) : m_R(r)
		{
		}

		void OnWindowResize(const WindowResizeEvent& event) override
		{
			m_R->ConstructFBO(Vec2(event.x, event.y), Vec2(event.width, event.height));
		}
	};
}

glib::PipelineRenderer::PipelineRenderer(const std::string& vertexCode, const std::string& fragmentCode) : m_VertexCode(vertexCode), m_FragmentCode(fragmentCode)
{
}

glib::PipelineRenderer::PipelineRenderer(const std::string& fragmentCode) : m_VertexCode(VERTEX_SHADER), m_FragmentCode(fragmentCode)
{
}

glib::PipelineRenderer::~PipelineRenderer()
{
	m_Wnd->GetEventManager().Unsubscribe(m_L);
	delete m_L;
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
}

void glib::PipelineRenderer::Construct(Window* wnd)
{
	wnd->SetToCurrentContext();
	m_L = new RendererListener(this);
	m_Wnd = wnd;
	wnd->GetEventManager().Subscribe(GLIB_EVENT_WINDOW_RESIZE, m_L);

	Vec2 size = wnd->GetInitialSize();
	m_Size = size;

	m_Shd = wnd->LoadShader(m_VertexCode, m_FragmentCode);
	
	ConstructFBO(Vec2(0.0f, 0.0f), size);

	float vertices[] = {
		// first triangle
		1.0f,  1.0f, 0.0f, 1.0, 1.0,  // top right
		1.0f,  0.0f, 0.0f, 1.0, 0.0,  // bottom right
		0.0f,  1.0f, 0.0f, 0.0, 1.0, // top left
		// second triangle
		1.0f,  0.0f, 0.0f, 1.0, 0.0, // bottom right
		0.0f,  0.0f, 0.0f, 0.0, 0.0, // bottom left
		0.0f,  1.0f, 0.0f, 0.0, 1.0 // top left
	};

	int verticeCount = 6;

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticeCount * 3 + sizeof(float) * verticeCount * 2, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void glib::PipelineRenderer::ConstructFBO(Vec2 pos, Vec2 size)
{
	m_Wnd->SetToCurrentContext();
	if (m_FB != nullptr)
	{
		delete m_FB;
	}

	m_Size = size;
	m_Pos = pos;

	Mat4 m = Mat4::Ortho(0.0f, m_Wnd->GetInitialSize().x, m_Wnd->GetInitialSize().y, 0.0f);
	m_Shd->Use();
	m_Shd->SetMat4("glib_projection_matrix", m);

	m_FB = new Framebuffer(FramebufferType::TEXTURE, size);
}
