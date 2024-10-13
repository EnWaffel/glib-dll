#include "glib/graphics/postprocessing/SpeedLinesEffect.h"
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
uniform float _time;
uniform float _radius;
uniform float _edge;
uniform vec4 _color;

in vec2 texCoord;

vec3 random3(vec3 c) {
	float j = 4096.0*sin(dot(c,vec3(17.0, 59.4, 15.0)));
	vec3 r;
	r.z = fract(512.0*j);
	j *= .125;
	r.x = fract(512.0*j);
	j *= .125;
	r.y = fract(512.0*j);
	return r-0.5;
}

float simplex3d(vec3 p) {
	 vec3 s = floor(p + dot(p, vec3(0.3333333)));
	 vec3 x = p - s + dot(s, vec3(0.1666667));
	 vec3 e = step(vec3(0.0), x - x.yzx);
	 vec3 i1 = e*(1.0 - e.zxy);
	 vec3 i2 = 1.0 - e.zxy*(1.0 - e);
	 vec3 x1 = x - i1 + 0.1666667;
	 vec3 x2 = x - i2 + 2.0*0.1666667;
	 vec3 x3 = x - 1.0 + 3.0*0.1666667;
	 vec4 w, d;
	 w.x = dot(x, x);
	 w.y = dot(x1, x1);
	 w.z = dot(x2, x2);
	 w.w = dot(x3, x3);
	 w = max(0.6 - w, 0.0);
	 d.x = dot(random3(s), x);
	 d.y = dot(random3(s + i1), x1);
	 d.z = dot(random3(s + i2), x2);
	 d.w = dot(random3(s + 1.0), x3);
	 w *= w;
	 w *= w;
	 d *= w;
	 return dot(d, vec4(52.0));
}

void main()
{
	vec2 iResolution = vec2(1.0, 1.0);
    float time = _time * 2.;
    float scale = 50.0;
    vec2 uv = (texCoord.xy*2. - iResolution.xy) / iResolution.y * 0.5;
    vec2 p = vec2(0.5*iResolution.x/iResolution.y, 0.5) + normalize(uv) * min(length(uv), 0.05);
    vec3 p3 = scale*0.25*vec3(p.xy, 0) + vec3(0, 0, time*0.025);
    float noise = simplex3d(p3 * 32.0) * 0.5 + 0.5;
    float dist = abs(clamp(length(uv)/_radius, 0.0, 1.0)*noise*2.-1.);
    float stepped = smoothstep(_edge-.5,_edge+.5, noise * (1.0-pow(dist, 4.0)));
    float final = smoothstep(_edge - 0.05, _edge + 0.05, noise*stepped);
    
    
	gl_FragColor = vec4(_color.rgb * final, 0.0) + texture(tex, texCoord);
}
)";

using namespace glib;

namespace glib
{
	static class SpeedLinesEffect_Listener : public EventSubscriber
	{
	private:
		SpeedLinesEffect* m_Parent;
	public:
		SpeedLinesEffect_Listener(SpeedLinesEffect* parent) : m_Parent(parent)
		{
		}

		void OnWindowResize(const WindowResizeEvent& event) override
		{
			m_Parent->ConstructFBO(Vec2(event.x, event.y), Vec2(event.width, event.height));
		}
	};
}

glib::SpeedLinesEffect::SpeedLinesEffect()
{
}

glib::SpeedLinesEffect::~SpeedLinesEffect()
{
	m_Wnd->GetEventManager().Unsubscribe((EventSubscriber*)m_L);
}

void glib::SpeedLinesEffect::SetRadius(float radius)
{
	m_Radius = radius;
	m_Wnd->SetToCurrentContext();
	m_Shd->Use();
	m_Shd->SetFloat("_radius", radius);
}

void glib::SpeedLinesEffect::SetEdge(float edge)
{
	m_Edge = edge;
	m_Wnd->SetToCurrentContext();
	m_Shd->Use();
	m_Shd->SetFloat("_edge", edge);
}

void glib::SpeedLinesEffect::SetColor(const Color& color)
{
	m_Color = color;
	m_Wnd->SetToCurrentContext();
	m_Shd->Use();
	m_Shd->SetColor("_color", color);
}

void glib::SpeedLinesEffect::Construct(Window* wnd)
{
	type = GLIB_PE_EFFECT_SPEED_LINES;

	m_Wnd = wnd;
	wnd->SetToCurrentContext();

	m_L = new SpeedLinesEffect_Listener(this);
	wnd->GetEventManager().Subscribe(GLIB_EVENT_WINDOW_RESIZE, (EventSubscriber*)m_L);

	m_Size = wnd->GetInitialSize();
	m_Shd = wnd->LoadShader(VERTEX_SHADER, FRAGMENT_SHADER);
	m_Shd->Use();
	m_Shd->SetFloat("_radius", 12.0f);
	m_Shd->SetFloat("_edge", 0.3f);
	m_Shd->SetColor("_color", { 1.0f, 1.0f, 1.0f, 1.0f });
	m_Radius = 12.0f;
	m_Edge = 0.3f;
	m_Color = { 1.0f, 1.0f, 1.0f, 1.0f };

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

const PipelineData glib::SpeedLinesEffect::Downstream(const PipelineData data)
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_FB->Bind();
	m_Shd->Use();
	m_Shd->SetFloat("_time", glfwGetTime());

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

void glib::SpeedLinesEffect::ConstructFBO(Vec2 pos, Vec2 size)
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
