#pragma once

#include "PipelineElement.h"
#include "../Shader.h"
#include "../../math/Mat4.h"

#define GLIB_PE_WINDOW_RENDERER 0x2

namespace glib
{
	class Listener;

	class WindowRenderer : public PipelineElement
	{
	private:
		Shader* m_Shd;
		unsigned int m_VAO;
		unsigned int m_VBO;
	public:
		GLIB_API WindowRenderer();
		~WindowRenderer();

		void Construct(Window* wnd);
		const PipelineData Downstream(const PipelineData data);
	};
}