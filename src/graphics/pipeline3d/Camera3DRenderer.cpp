#include "glib/graphics/pipeline3d/Camera3DRenderer.h"
#include <vector>
#include "glib/graphics/camera/Camera3D.h"
#include "glib/window/Window.h"
#include <iostream>
#include "glib/graphics/3d/Object3D.h"
#include "glib/math/MathFunctions.h"

#include <memory>
#include <glad/glad.h>

using namespace glib;

static const char* VERTEX_SHADER = R"(
#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 2) in vec2 _texCoord;

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

uniform sampler2D glib_texture_diffuse1;
uniform vec4 glib_color;
uniform vec2 glib_uv_coord;
uniform vec2 glib_uv_size;
uniform bool useColor;

void main()
{
	if (useColor)
	{
		gl_FragColor = glib_color;
	}
	else
	{
		gl_FragColor = texture(glib_texture_diffuse1, glib_uv * glib_uv_size + glib_uv_coord);
	}
}
)";

glib::Camera3DRenderer::Camera3DRenderer() : glib::PipelineRenderer(VERTEX_SHADER, FRAGMENT_SHADER)
{
}

glib::Camera3DRenderer::~Camera3DRenderer()
{
}

void glib::Camera3DRenderer::Construct(Window* wnd)
{
	glib::PipelineRenderer::Construct(wnd);
	type = GLIB_PE_CAMERA_RENDERER_3D;
}

static float rot = 0.0f;

const PipelineData glib::Camera3DRenderer::Downstream(const PipelineData data)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	Camera3D* cam = (Camera3D*)data.ptr;

	glViewport(viewportPos.x, viewportPos.y, viewportSize.x, viewportSize.y);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_Shd->Use();

	Mat4 m = cam->CalculateView();

	m_Shd->SetMat4("glib_view_matrix", m);

	for (Drawable* d : cam->GetDrawables())
	{
		if (!d->visible) continue;
		Object3D* obj = dynamic_cast<Object3D*>(d);
		if (!obj) continue;
		if (obj->model == nullptr) continue;

		m_Shd->SetColor("glib_color", obj->color);

		Mat4 modelMat;
		modelMat.Translate(obj->pos);
		modelMat.Scale(obj->scale);
		
		modelMat.Translate(obj->pivotPoint);
		modelMat.Rotate(obj->rotation.x, glib::Vec3(1.0f, 0.0f, 0.0f));
		modelMat.Rotate(obj->rotation.y, glib::Vec3(0.0f, 1.0f, 0.0f));
		modelMat.Rotate(obj->rotation.z, glib::Vec3(0.0f, 0.0f, 1.0f));
		modelMat.Translate(-obj->pivotPoint);

		m_Shd->SetMat4("glib_model_matrix", modelMat);
		m_Shd->SetVec2("glib_uv_pos", obj->uvPos);
		m_Shd->SetVec2("glib_uv_size", obj->uvSize);

		for (Mesh* mesh : obj->model->m_Meshes)
		{
			unsigned int diffuseNr = 1;
			unsigned int specularNr = 1;
			unsigned int normalNr = 1;
			unsigned int heightNr = 1;
			
			m_Shd->SetInt("useColor", mesh->m_Textures.size() < 1);
			
			for (unsigned int i = 0; i < mesh->m_Textures.size(); i++)
			{
				glActiveTexture(GL_TEXTURE0 + i);
				MeshTexture& tex = mesh->m_Textures[i];

				std::string num;

				switch (tex.type)
				{
				case GLIB_MESH_TEX_TYPE_DIFFUSE:
				{
					num = "glib_texture_diffuse" + std::to_string(diffuseNr);
					diffuseNr++;
					break;
				}
				case GLIB_MESH_TEX_TYPE_SPECULAR:
				{
					num = "glib_texture_specular" + std::to_string(specularNr);
					specularNr++;
					break;
				case GLIB_MESH_TEX_TYPE_NORMAL:
				{
					num = "glib_texture_normal" + std::to_string(normalNr);
					normalNr++;
					break;
				}
				case GLIB_MESH_TEX_TYPE_HEIGHT:
				{
					num = "glib_texture_height" + std::to_string(heightNr);
					heightNr++;
					break;
				}
				}
				}

				m_Shd->SetInt(num, i);
				glBindTexture(GL_TEXTURE_2D, tex.id);
			}

			glBindVertexArray(mesh->m_VAO);
			glDrawElements(GL_TRIANGLES, mesh->m_Indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			glActiveTexture(GL_TEXTURE0);
		}
	}

	rot += 0.16f;

	return { cam, m_FB->GetInternals().tex };
}

void glib::Camera3DRenderer::ConstructFBO(Vec2 pos, Vec2 size)
{
	PipelineRenderer::ConstructFBO(pos, size);

	Mat4 m = Mat4::Perspective(glib::ToRadians(45.0f), m_Wnd->GetInitialSize().x / m_Wnd->GetInitialSize().y, 0.001f, 10000.0f);
	m_Shd->Use();
	m_Shd->SetMat4("glib_projection_matrix", m);
}

/*
	for (Drawable* d : cam->GetDrawables())
	{
		if (!d->visible) continue;
		Object3D* obj = dynamic_cast<Object3D*>(d);
		if (!obj) continue;
		if (obj->model == nullptr) continue;

		std::cout << "model!\n";

		for (Mesh* mesh : obj->model->m_Meshes)
		{
			unsigned int diffuseNr = 1;
			unsigned int specularNr = 1;
			unsigned int normalNr = 1;
			unsigned int heightNr = 1;
			for (unsigned int i = 0; i < mesh->m_Textures.size(); i++)
			{
				std::cout << "tex idx: " << i << std::endl;
				std::cout << "(" << __LINE__ << ")" << glGetError() << std::endl;
				glActiveTexture(GL_TEXTURE0 + i);
				MeshTexture& tex = mesh->m_Textures[i];

				std::string num;

				switch (tex.type)
				{
				case GLIB_MESH_TEX_TYPE_DIFFUSE:
				{
					std::cout << "diffuse tex!\n";
					num = "glib_texture_diffuse" + std::to_string(diffuseNr);
					diffuseNr++;
					break;
				}
				case GLIB_MESH_TEX_TYPE_SPECULAR:
				{
					num = "glib_texture_specular" + std::to_string(specularNr);
					specularNr++;
					break;
				case GLIB_MESH_TEX_TYPE_NORMAL:
				{
					num = "glib_texture_normal" + std::to_string(normalNr);
					normalNr++;
					break;
				}
				case GLIB_MESH_TEX_TYPE_HEIGHT:
				{
					num = "glib_texture_height" + std::to_string(heightNr);
					heightNr++;
					break;
				}
				}
				}

				//m_Shd->SetInt(num, i);
				std::cout << "(" << __LINE__ << ")" << glGetError() << std::endl;
				glBindTexture(GL_TEXTURE_2D, tex.id);
				std::cout << "(" << __LINE__ << ")" << glGetError() << std::endl;
			}

			glBindVertexArray(mesh->m_VAO);
			std::cout << mesh->m_VAO << std::endl;
			std::cout << "(" << __LINE__ << ")" << glGetError() << std::endl;
			glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(mesh->m_Indices.size()), GL_UNSIGNED_INT, 0);
			std::cout << "(" << __LINE__ << ")" << glGetError() << std::endl;
			glBindVertexArray(0);

			glActiveTexture(GL_TEXTURE0);
		}
	}
	*/
