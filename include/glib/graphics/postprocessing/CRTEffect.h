#pragma once

#include "../../DLLDefs.h"
#include "../pipeline/PipelineElement.h"
#include "../../framebuffer/Framebuffer.h"
#include "../Shader.h"

#define GLIB_PE_EFFECT_CRT 0x3

namespace glib
{
	class CRTEffect : public PipelineElement
	{
	private:
		Framebuffer* m_FB;
		void* m_L;
		Window* m_Wnd;
		Vec2 m_Pos;
		Vec2 m_Size;
		Shader* m_Shd;
		unsigned int m_VAO;
		unsigned int m_VBO;
	public:
		CRTEffect();
		~CRTEffect();

		void Construct(Window* wnd) override;
		const PipelineData Downstream(const PipelineData data) override;
		void ConstructFBO(Vec2 pos, Vec2 size);
	};
}