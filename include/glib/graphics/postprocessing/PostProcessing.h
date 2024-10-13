#pragma once

#include "../../DLLDefs.h"
#include "../pipeline/RenderPipeline.h"

#include <vector>
#include <string>

#define GLIB_PP_BLOOM 0x0001
#define GLIB_PP_CRT_TV 0x0002
#define GLIB_PP_CHROMATIC 0x0004
#define GLIB_PP_SPEED_LINES 0x0008
#define GLIB_PP_CUSTOM_SHADER 0x0010

namespace glib
{

	struct ShaderEffectParams {};

	struct BloomParams : ShaderEffectParams
	{
		float strength;
	};

	struct SpeedLinesParams : ShaderEffectParams
	{

	};

	struct ChromaticParams : ShaderEffectParams
	{
		float strength = 0.0f;
	};

	struct CustomShaderParams : ShaderEffectParams
	{
		std::string fragmentCode;
	};

	class PostProcessing
	{
	public:
		class Builder
		{
		private:
			RenderPipeline* m_Pipeline;
			bool m_Built;
			Window* m_Wnd;
		public:
			Builder(Window* wnd);
			GLIB_API ~Builder();

			GLIB_API Builder& AddEffect(uint64_t effect, const ShaderEffectParams& params);
			GLIB_API RenderPipeline* Build();
		};
	public:
		PostProcessing() = delete;
		~PostProcessing() = delete;

		GLIB_API static Builder NewBuilder(Window* wnd);
	};
}