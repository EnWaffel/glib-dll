#include "glib/graphics/postprocessing/CustomShader.h"
#include <glad/glad.h>
#include <glib/window/Window.h>
#include <glib/math/Mat4.h>

static const char* VERTEX_SHADER = R"(
#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 _texCoord;

out vec2 glib_uv;

void main()
{
	gl_Position = vec4(pos, 1.0);
	glib_uv = _texCoord;
}
)";

using namespace glib;

glib::CustomShader::CustomShader(const std::string& fragmentCode) : PipelineRenderer(VERTEX_SHADER, fragmentCode)
{
}

glib::CustomShader::~CustomShader()
{
}

void glib::CustomShader::Construct(Window* wnd)
{
	PipelineRenderer::Construct(wnd);

	type = GLIB_PE_CUSTOM_SHADER;

	glDeleteBuffers(1, &m_VAO);
	glDeleteVertexArrays(1, &m_VBO);

	float vertices[] = {
		// first triangle
		1.0f,  1.0f, 0.0f, 1.0, 1.0,  // top right
		1.0f,  -1.0f, 0.0f, 1.0, 0.0,  // bottom right
		-1.0f,  1.0f, 0.0f, 0.0, 1.0, // top left
		// second triangle
		1.0f,  -1.0f, 0.0f, 1.0, 0.0, // bottom right
		-1.0f,  -1.0f, 0.0f, 0.0, 0.0, // bottom left
		-1.0f,  1.0f, 0.0f,  0.0, 1.0 // top left
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

const glib::PipelineData glib::CustomShader::Downstream(const PipelineData data)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_FB->Bind();
	m_Shd->Use();

	glViewport(0, 0, m_Size.x, m_Size.y);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, data.uI);

	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);

	m_FB->Unbind();

	return { data.ptr, m_FB->GetInternals().tex };
}

void glib::CustomShader::ConstructFBO(Vec2 pos, Vec2 size)
{
	PipelineRenderer::ConstructFBO(pos, size);
	m_Shd->SetVec2("texSize", size);
}

Shader* glib::CustomShader::GetShader()
{
	return m_Shd;
}
