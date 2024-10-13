#include "glib/graphics/postprocessing/CRTEffect.h"
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
#version 330 compatibility

uniform sampler2D tex;
uniform float time;

in vec2 texCoord;

vec2 curve(vec2 uv)
{
	uv = (uv - 0.5) * 2.0;
	uv *= 1.1;	
	uv.x *= 1.0 + pow((abs(uv.y) / 5.0), 2.0);
	uv.y *= 1.0 + pow((abs(uv.x) / 4.0), 2.0);
	uv  = (uv / 2.0) + 0.5;
	uv =  uv *0.92 + 0.04;
	return uv;
}
void main()
{
    vec2 q = texCoord;
    vec2 uv = q;
    uv = curve( uv );
    vec3 oricol = texture( tex, vec2(q.x,q.y) ).rgb;
    vec3 col;
	float x =  sin(0.3*time+uv.y*21.0)*sin(0.7*time+uv.y*29.0)*sin(0.3+0.33*time+uv.y*31.0)*0.0017;

    col.r = texture(tex,vec2(x+uv.x+0.001,uv.y+0.001)).x+0.05;
    col.g = texture(tex,vec2(x+uv.x+0.000,uv.y-0.002)).y+0.05;
    col.b = texture(tex,vec2(x+uv.x-0.002,uv.y+0.000)).z+0.05;
    col.r += 0.08*texture(tex,0.75*vec2(x+0.025, -0.027)+vec2(uv.x+0.001,uv.y+0.001)).x;
    col.g += 0.05*texture(tex,0.75*vec2(x+-0.022, -0.02)+vec2(uv.x+0.000,uv.y-0.002)).y;
    col.b += 0.08*texture(tex,0.75*vec2(x+-0.02, -0.018)+vec2(uv.x-0.002,uv.y+0.000)).z;

    col = clamp(col*0.6+0.4*col*col*1.0,0.0,1.0);

    float vig = (0.0 + 1.0*16.0*uv.x*uv.y*(1.0-uv.x)*(1.0-uv.y));
	col *= vec3(pow(vig,0.3));

    col *= vec3(0.95,1.05,0.95);
	col *= 2.8;

	float scans = clamp( 0.35+0.35*sin(3.5*time+uv.y*1.5), 0.0, 1.0);
	
	float s = pow(scans,1.7);
	col = col*vec3( 0.4+0.7*s) ;

    col *= 1.0+0.01*sin(110.0*time);
	if (uv.x < 0.0 || uv.x > 1.0)
		col *= 0.0;
	if (uv.y < 0.0 || uv.y > 1.0)
		col *= 0.0;
	
	//col*=1.0-0.65*vec3(clamp((mod(texCoord.x, 2.0)-1.0)*2.0,0.0,1.0));
	
    float comp = smoothstep( 0.1, 0.9, sin(time) );
 
	// Remove the next line to stop cross-fade between original and postprocess
//	col = mix( col, oricol, comp );

    gl_FragColor = vec4(col, texture( tex, vec2(q.x,q.y) ).a);
}
)";

using namespace glib;

namespace glib
{
	static class CRTEffect_Listener : public EventSubscriber
	{
	private:
		CRTEffect* m_Parent;
	public:
		CRTEffect_Listener(CRTEffect* parent) : m_Parent(parent)
		{
		}

		void OnWindowResize(const WindowResizeEvent& event) override
		{
			m_Parent->ConstructFBO(Vec2(event.x, event.y), Vec2(event.width, event.height));
		}
	};
}

glib::CRTEffect::CRTEffect()
{
}

glib::CRTEffect::~CRTEffect()
{
	m_Wnd->GetEventManager().Unsubscribe((EventSubscriber*)m_L);
}

void glib::CRTEffect::Construct(Window* wnd)
{
	type = GLIB_PE_EFFECT_CRT;

	m_Wnd = wnd;
	wnd->SetToCurrentContext();

	m_L = new CRTEffect_Listener(this);
	wnd->GetEventManager().Subscribe(GLIB_EVENT_WINDOW_RESIZE, (EventSubscriber*)m_L);

	m_Size = wnd->GetInitialSize();
	m_Shd = wnd->LoadShader(VERTEX_SHADER, FRAGMENT_SHADER);

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

const PipelineData glib::CRTEffect::Downstream(const PipelineData data)
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_FB->Bind();
	m_Shd->Use();
	m_Shd->SetFloat("time", glfwGetTime());

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

void glib::CRTEffect::ConstructFBO(Vec2 pos, Vec2 size)
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
