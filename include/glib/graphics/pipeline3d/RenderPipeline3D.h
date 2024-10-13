#pragma once

#include <vector>

#include "../../DLLDefs.h"
#include "../camera/Camera.h"
#include "../camera/Camera3D.h"
#include "../pipeline/PipelineElement.h"
#include "../pipeline3d/Camera3DRenderer.h"
#include "../pipeline/RenderPipeline.h"

namespace glib
{
	class Window;

	class RenderPipeline3D : public RenderPipeline
	{
	private:
		Camera3D* m_Cam3D;
		Camera3DRenderer* m_Renderer3D;
	public:
		GLIB_API RenderPipeline3D(Window* wnd);
		GLIB_API ~RenderPipeline3D() override;

		GLIB_API void Update(float delta) override;

		GLIB_API void Flush(const std::vector<Camera*>& cameras) const override;
		GLIB_API Camera3D* GetCamera();
	};
}