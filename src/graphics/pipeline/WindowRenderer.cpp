#include "glib/graphics/pipeline/WindowRenderer.h"
#include "glib/window/Window.h"

#include <glad/glad.h>

using namespace glib;
static const char* VERTEX_SHADER = R"(
#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 _texCoord;

out vec2 texCoord;

void main()
{
	gl_Position = vec4(pos, 1.0);
	texCoord = _texCoord;
}
)";

static const char* FRAGMENT_SHADER = R"(
#version 330 core

uniform sampler2D tex;

in vec2 texCoord;

void main()
{
	gl_FragColor = texture(tex, texCoord);
}
)";

glib::WindowRenderer::WindowRenderer() : m_Shd(nullptr), m_VAO(0), m_VBO(0)
{
}

glib::WindowRenderer::~WindowRenderer()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
}


void glib::WindowRenderer::Construct(Window* wnd)
{
	type = GLIB_PE_WINDOW_RENDERER;
	glDisable(GL_BLEND);

	m_Shd = wnd->LoadShader(VERTEX_SHADER, FRAGMENT_SHADER);
	m_Shd->Use();

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

const PipelineData glib::WindowRenderer::Downstream(const PipelineData data)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_Shd->Use();

	glViewport(viewportPos.x, viewportPos.y, viewportSize.x, viewportSize.y);

	glBindTexture(GL_TEXTURE_2D, data.uI);

	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
	return {};
}