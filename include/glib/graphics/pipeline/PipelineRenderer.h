#pragma once

#include "PipelineElement.h"
#include "../Shader.h"
#include "../../math/Mat4.h"
#include "../../framebuffer/Framebuffer.h"
#include "../Sprite.h"
#include "../Text.h"
#include "../../DLLDefs.h"

namespace glib
{
	class RendererListener;

	class PipelineRenderer : public PipelineElement
	{
	protected:
		Shader* m_Shd;
		unsigned int m_VAO;
		unsigned int m_VBO;
		Framebuffer* m_FB;
		Vec2 m_Size;
		Vec2 m_Pos;
		RendererListener* m_L;
		Window* m_Wnd;
		Mat4 m_Proj;
		std::string m_VertexCode;
		std::string m_FragmentCode;
	public:
		GLIB_API PipelineRenderer(const std::string& vertexCode, const std::string& fragmentCode);
		GLIB_API PipelineRenderer(const std::string& fragmentCode);
		GLIB_API virtual ~PipelineRenderer();

		GLIB_API virtual void Construct(Window* wnd) override;
		GLIB_API virtual const PipelineData Downstream(const PipelineData data) = 0;
		GLIB_API virtual void ConstructFBO(Vec2 pos, Vec2 size);
	};
}