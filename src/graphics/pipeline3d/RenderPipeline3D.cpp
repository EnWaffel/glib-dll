#include "glib/graphics/pipeline3d/RenderPipeline3D.h"
#include "glib/window/Window.h"
#include <iostream>
#include "glib/graphics/pipeline/WindowRenderer.h"

using namespace glib;

glib::RenderPipeline3D::RenderPipeline3D(Window* wnd) : RenderPipeline(wnd)
{
	m_Cam3D = new Camera3D(wnd->GetInitialSize(), wnd);
	m_Renderer3D = new Camera3DRenderer;
	m_Renderer3D->Construct(wnd);
}

glib::RenderPipeline3D::~RenderPipeline3D()
{
	delete m_Cam3D;
	delete m_Renderer3D;
}

void glib::RenderPipeline3D::Update(float delta)
{
	m_Cam3D->Update(delta);
}

Camera3D* glib::RenderPipeline3D::GetCamera()
{
	return m_Cam3D;
}

void glib::RenderPipeline3D::Flush(const std::vector<Camera*>& cameras) const
{
	const Vec2& viewportPos = m_Wnd->GetViewportPos();
	const Vec2& viewportSize = m_Wnd->GetViewportSize();

	for (PipelineElement* element : m_Elements)
	{
		element->viewportPos.x = viewportPos.x;
		element->viewportPos.y = viewportPos.y;
		element->viewportSize.x = viewportSize.x;
		element->viewportSize.y = viewportSize.y;
	}

	m_Renderer3D->viewportPos = viewportPos;
	m_Renderer3D->viewportSize = viewportSize;

	m_Renderer3D->Downstream({ m_Cam3D, 0, 0, 0, m_Wnd });

	for (Camera* camera : cameras)
	{
		PipelineData data{};
		data.ptr = camera;
		data.wnd = m_Wnd;

		for (PipelineElement* element : m_Elements)
		{
			if (element->IsExcludedFromCamera(camera)) continue;
			data = element->Downstream(data);
		}
	}
}
