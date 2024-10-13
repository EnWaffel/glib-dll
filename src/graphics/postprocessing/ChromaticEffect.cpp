#include "glib/graphics/postprocessing/ChromaticEffect.h"
#include "glib/event/EventManager.h"
#include "glib/window/Window.h"
#include <glad/glad.h>
#include <iostream>
#include <GLFW/glfw3.h>

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
uniform float strength;

in vec2 texCoord;

void main()
{
vec2 U = texCoord;
	vec2 R = vec2(-1.0, 1.0), m = vec2(strength, 1.0).xy; 
	
    float d = (length(m)<.02) ? .015 : m.x/10.;
 
	gl_FragColor = vec4( texture(tex,U-d).x,
              texture(tex,U  ).y,
              texture(tex,U+d).z,
              texture(tex, U).a);
}
)";

using namespace glib;

namespace glib
{
	static class CRTEffect_Listener : public EventSubscriber
	{
	private:
		ChromaticEffect* m_Parent;
	public:
		CRTEffect_Listener(ChromaticEffect* parent) : m_Parent(parent)
		{
		}

		void OnWindowResize(const WindowResizeEvent& event) override
		{
			m_Parent->ConstructFBO(Vec2(event.x, event.y), Vec2(event.width, event.height));
		}
	};
}

glib::ChromaticEffect::ChromaticEffect(const ChromaticParams& params)
{
	m_Strength = params.strength;
}

glib::ChromaticEffect::~ChromaticEffect()
{
	m_Wnd->GetEventManager().Unsubscribe((EventSubscriber*)m_L);
}

void glib::ChromaticEffect::SetStrength(float strength)
{
	m_Strength = strength;
	m_Wnd->SetToCurrentContext();
	m_Shd->Use();
	m_Shd->SetFloat("strength", strength);
}

void glib::ChromaticEffect::Construct(Window* wnd)
{
	type = GLIB_PE_EFFECT_CHROMATIC;

	m_Wnd = wnd;
	wnd->SetToCurrentContext();

	m_L = new CRTEffect_Listener(this);
	wnd->GetEventManager().Subscribe(GLIB_EVENT_WINDOW_RESIZE, (EventSubscriber*)m_L);

	m_Size = wnd->GetInitialSize();
	m_Shd = wnd->LoadShader(VERTEX_SHADER, FRAGMENT_SHADER);
	m_Shd->Use();
	m_Shd->SetFloat("strength", m_Strength);

	ConstructFBO(glib::Vec2(0.0f, 0.0f), wnd->GetInitialSize());

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

const PipelineData glib::ChromaticEffect::Downstream(const PipelineData data)
{
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

void glib::ChromaticEffect::ConstructFBO(Vec2 pos, Vec2 size)
{
	m_Wnd->SetToCurrentContext();
	if (m_FB != nullptr)
	{
		delete m_FB;
	}

	m_Size = size;
	m_Pos = pos;

	m_FB = new Framebuffer(FramebufferType::TEXTURE, size);
}
