#include "glib/graphics/pipeline/RenderPipeline.h"
#include "glib/window/Window.h"
#include <iostream>

using namespace glib;

glib::RenderPipeline::RenderPipeline(Window* wnd) : m_Wnd(wnd)
{
}

glib::RenderPipeline::~RenderPipeline()
{
	for (PipelineElement* element : m_Elements)
	{
		delete element;
	}
}

void glib::RenderPipeline::Update(float delta)
{
}

void glib::RenderPipeline::AddElement(PipelineElement* element)
{
	element->Construct(m_Wnd);
	m_Elements.push_back(element);
}

void glib::RenderPipeline::RemoveElement(PipelineElement* element)
{
	m_Elements.erase(std::find(m_Elements.begin(), m_Elements.end(), element));
}

PipelineElement* glib::RenderPipeline::GetElementByType(uint8_t type) const
{
	for (PipelineElement* element : m_Elements)
	{
		if (element->type == type) return element;
	}
	return nullptr;
}

void glib::RenderPipeline::Flush(const std::vector<Camera*>& cameras) const
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
