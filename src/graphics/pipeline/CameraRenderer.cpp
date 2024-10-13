#include "glib/graphics/pipeline/CameraRenderer.h"
#include <vector>
#include "glib/graphics/camera/Camera.h"
#include "glib/window/Window.h"
#include "glib/graphics/Sprite.h"
#include <iostream>

#include <memory>
#include <glad/glad.h>

using namespace glib;

static const char* VERTEX_SHADER = R"(
#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 _texCoord;

out vec2 glib_uv;

uniform mat4 glib_projection_matrix;
uniform mat4 glib_view_matrix;
uniform mat4 glib_model_matrix;

void main()
{
	gl_Position = glib_projection_matrix * glib_view_matrix * glib_model_matrix * vec4(pos, 1.0);
	glib_uv = _texCoord;
}
)";

static const char* FRAGMENT_SHADER = R"(
#version 330 core

in vec2 glib_uv;

uniform sampler2D glib_texture;
uniform vec4 glib_color;
uniform vec2 glib_uv_coord;
uniform vec2 glib_uv_size;

void main()
{
	vec4 c = texture(glib_texture, glib_uv * glib_uv_size + glib_uv_coord) * glib_color;
	gl_FragColor = c;
}
)";

static const char* TEXT_FRAGMENT_SHADER = R"(
#version 330 core

in vec2 glib_uv;

uniform sampler2D glib_texture;
uniform vec4 glib_color;

void main()
{
    vec2 c = vec2(glib_uv.x, glib_uv.y);
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(glib_texture, c).r);
    gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0) * sampled;
    gl_FragColor *= glib_color;
}
)";

glib::CameraRenderer::CameraRenderer() : glib::PipelineRenderer(VERTEX_SHADER, FRAGMENT_SHADER)
{
}

glib::CameraRenderer::~CameraRenderer()
{
}

void glib::CameraRenderer::Construct(Window* wnd)
{
	m_TextShd = wnd->LoadShader(VERTEX_SHADER, TEXT_FRAGMENT_SHADER);
	glib::PipelineRenderer::Construct(wnd);

	type = GLIB_PE_CAMERA_RENDERER;

	float vertices[] = {
		// first triangle
		1.0f,  1.0f, 0.0f, 0.0, 1.0,  // top right
		1.0f,  0.0f, 0.0f, 0.0, 0.0,  // bottom right
		0.0f,  1.0f, 0.0f, 1.0, 1.0, // top left
		// second triangle
		1.0f,  0.0f, 0.0f, 0.0, 0.0, // bottom right
		0.0f,  0.0f, 0.0f, 1.0, 0.0, // bottom left
		0.0f,  1.0f, 0.0f, 1.0, 1.0 // top left
	};

	int verticeCount = 6;

	glGenVertexArrays(1, &m_VAOx);
	glGenBuffers(1, &m_VBOx);

	glBindVertexArray(m_VAOx);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOx);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticeCount * 3 + sizeof(float) * verticeCount * 2, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

const PipelineData glib::CameraRenderer::Downstream(const PipelineData data)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Camera* cam = (Camera*) data.ptr;

	m_FB->Bind();
	glViewport(0, 0, m_Size.x, m_Size.y);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	Mat4 m = cam->CalculateView();

	for (Drawable* d : cam->GetDrawables())
	{
		if (!d->visible) continue;

		Sprite* s = dynamic_cast<Sprite*>(d);
		Text* t = dynamic_cast<Text*>(d);
		VideoPlayer* p = dynamic_cast<VideoPlayer*>(d);

		if (s)
		{
			m_Shd->Use();
			m_Shd->SetMat4("glib_view_matrix", m);
			DrawSprite(s, cam->pos, cam->zoom);
		}
		else if (t)
		{
			m_TextShd->Use();
			m_TextShd->SetMat4("glib_view_matrix", m);
			DrawText(t, cam->pos, cam->zoom);
		}
		else if (p)
		{
			m_Shd->Use();
			m_Shd->SetMat4("glib_view_matrix", m);
			DrawVideoPlayer(p);
		}
		else
		{
			d->Draw();
		}
	}

	m_FB->Unbind();

	return { cam, m_FB->GetInternals().tex };
}

void glib::CameraRenderer::DrawSprite(Sprite* s, const Vec2& cameraPos, float zoom)
{
	m_Shd->Use();

	Vec2 pos = s->pos;
	pos.x -= cameraPos.x * s->scrollFactor.x;
	pos.y -= cameraPos.y * s->scrollFactor.y;

	Mat4 modelMat;
	Vec2 diff = Vec2((s->size.x * s->scale.x) - s->size.x, (s->size.y * s->scale.y) - s->size.y);

	modelMat.Translate(Vec2(pos.x - (diff.x / 2.0f), pos.y - (diff.y / 2.0f)));

	modelMat.Translate(Vec2(s->size.x / 2.0f * s->scale.x, s->size.y / 2.0f * s->scale.y));
	modelMat.Rotate(s->rotation);
	modelMat.Translate(Vec2(-(s->size.x / 2.0f) * s->scale.x, -(s->size.y / 2.0f) * s->scale.y));
	modelMat.Translate(s->offset);

	modelMat.Scale(Vec2(s->size.x + diff.x, s->size.y + diff.y));

	m_Shd->SetMat4("glib_model_matrix", modelMat);
	m_Shd->SetColor("glib_color", s->color);
	m_Shd->SetVec2("glib_uv_coord", s->textureOffset);
	m_Shd->SetVec2("glib_uv_size", s->textureSize);

	if (s->tex != nullptr)
	{
		m_Shd->SetInt("glib_texture", 0);
		glActiveTexture(GL_TEXTURE0);
		s->tex->Bind();
	}

	if (s->flipX)
	{
		glBindVertexArray(m_VAOx);
	}
	else
	{
		glBindVertexArray(m_VAO);
	}
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	if (s->tex != nullptr)
	{
		s->tex->Unbind();
	}
}

void glib::CameraRenderer::DrawText(Text* text, const Vec2& cameraPos, float zoom)
{
	m_TextShd->Use();
	float xOffset = 0.0f;

	for (wchar_t c : text->GetText())
	{
		const Glyph& glyph = text->font->GetGlyph(c);

		Vec2 pos = text->pos;
		pos.x -= cameraPos.x * (1 * zoom);
		pos.y -= cameraPos.y * (1 * zoom);

		float xpos = pos.x + xOffset + glyph.bearing.x * text->scale;
		float ypos = pos.y - glyph.bearing.y * text->scale;

		float w = glyph.size.x * text->scale;
		float h = glyph.size.y * text->scale;

		Mat4 model;

		model.Translate(Vec2(xpos, ypos));
		model.Translate(text->offset);

		model.Translate(Vec2(w / 2.0f, h / 2.0f));
		model.Rotate(text->rotation);
		model.Translate(Vec2(-(w / 2.0f), -(h / 2.0f)));

		model.Scale(Vec2(w, h));

		m_TextShd->Use();
		m_TextShd->SetColor("glib_color", text->color);
		m_TextShd->SetMat4("glib_model_matrix", model);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, glyph.tex);

		glBindVertexArray(m_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		xOffset += (glyph.advance >> 6) * text->scale;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

void glib::CameraRenderer::DrawVideoPlayer(VideoPlayer* player)
{
	Mat4 modelMat;
	Vec2 diff = Vec2((player->size.x * player->scale.x) - player->size.x, (player->size.y * player->scale.y) - player->size.y);

	modelMat.Translate(Vec2(player->pos.x - (diff.x / 2.0f), player->pos.y - (diff.y / 2.0f)));

	modelMat.Rotate(player->rotation);

	modelMat.Scale(Vec2(player->size.x + diff.x, player->size.y + diff.y));

	m_Shd->Use();
	m_Shd->SetMat4("glib_model_matrix", modelMat);
	m_Shd->SetColor("glib_color", { 1.0f, 1.0f, 1.0f, 1.0f });
	m_Shd->SetVec2("glib_uv_coord", glib::Vec2(0.0f, 0.0f));
	m_Shd->SetVec2("glib_uv_size", glib::Vec2(1.0f, 1.0f));

	glActiveTexture(GL_TEXTURE0);
	player->Draw();

	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void glib::CameraRenderer::ConstructFBO(Vec2 pos, Vec2 size)
{
	glib::PipelineRenderer::ConstructFBO(pos, size);

	Mat4 m = Mat4::Ortho(0.0f, m_Wnd->GetInitialSize().x, m_Wnd->GetInitialSize().y, 0.0f);
	m_TextShd->Use();
	m_TextShd->SetMat4("glib_projection_matrix", m);
}
