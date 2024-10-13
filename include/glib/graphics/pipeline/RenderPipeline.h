#pragma once

#include <vector>

#include "../../DLLDefs.h"
#include "../camera/Camera.h"
#include "PipelineElement.h"

namespace glib
{
	class Window;

	class RenderPipeline
	{
	protected:
		std::vector<PipelineElement*> m_Elements;
		Window* m_Wnd;
	public:
		GLIB_API RenderPipeline(Window* wnd);
		GLIB_API virtual ~RenderPipeline();

		GLIB_API virtual void Update(float delta);

		GLIB_API virtual void Flush(const std::vector<Camera*>& cameras) const;
		GLIB_API virtual void AddElement(PipelineElement* element);
		GLIB_API virtual void RemoveElement(PipelineElement* element);
		GLIB_API virtual PipelineElement* GetElementByType(uint8_t type) const;
	};
}