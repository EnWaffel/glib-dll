#pragma once

#include "../../DLLDefs.h"
#include "../pipeline/PipelineElement.h"
#include "../../framebuffer/Framebuffer.h"
#include "../Shader.h"
#include "PostProcessing.h"

#define GLIB_PE_EFFECT_CHROMATIC 0x4

namespace glib
{
	class ChromaticEffect : public PipelineElement
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
		float m_Strength;
	public:
		ChromaticEffect(const ChromaticParams& params);
		~ChromaticEffect();

		GLIB_API void SetStrength(float strength);

		void Construct(Window* wnd) override;
		const PipelineData Downstream(const PipelineData data) override;
		void ConstructFBO(Vec2 pos, Vec2 size);
	};
}