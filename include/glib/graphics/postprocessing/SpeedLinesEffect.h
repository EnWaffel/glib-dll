#pragma once

#include "../../DLLDefs.h"
#include "../pipeline/PipelineElement.h"
#include "../../framebuffer/Framebuffer.h"
#include "../Shader.h"
#include "../../utils/Color.h"

#define GLIB_PE_EFFECT_SPEED_LINES 0x5

namespace glib
{
	class SpeedLinesEffect : public PipelineElement
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
		float m_Radius;
		float m_Edge;
		Color m_Color;
	public:
		SpeedLinesEffect();
		~SpeedLinesEffect();

		GLIB_API void SetRadius(float radius);
		GLIB_API void SetEdge(float edge);
		GLIB_API void SetColor(const Color& color);

		void Construct(Window* wnd) override;
		const PipelineData Downstream(const PipelineData data) override;
		void ConstructFBO(Vec2 pos, Vec2 size);
	};
}